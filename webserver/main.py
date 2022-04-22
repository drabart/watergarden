from flask import Flask, flash, redirect, render_template, request, session, abort, url_for, send_from_directory
from passlib.hash import sha256_crypt
import mysql.connector as mariadb
from os import urandom
import operator

app = Flask(__name__)
db = mariadb.connect(user='', password='', database='Login')

def render_index():
  cur = db.cursor()
  cur.execute("SELECT * FROM variables")
  ret = cur.fetchall()
  dict = {}

  for var in ret:
    dict[var[1]] = var[3]

  return render_template('index.html', dict=dict)

@app.route('/', methods=['GET'])
def home():
  if not session.get('logged_in'):
    return redirect(url_for('display_login_page'))
  else:
    return render_index()

def db_change(name):
  cur = db.cursor()
  cur.execute(f"SELECT value FROM variables WHERE name = '{name}'")
  ret = cur.fetchall()[0][0]

  if ret == 'false':
    cur.execute(f"UPDATE variables SET value = 'true' WHERE name = '{name}'")
  else:
    cur.execute(f"UPDATE variables SET value = 'false' WHERE name = '{name}'")
  db.commit()
  cur.close()

@app.route('/', methods=['POST'])
def set_variable_logged():
  if not session.get('logged_in'):
    return home()

  variable = None
  button = request.form
  variable = list(button.lists())[0][0]

  if variable is not None:
    db_change(variable)

  return render_index()

def check_password(login):
  userName = login['username']
  # password with salt
  password = login['password'] + "salt"

  if userName is None:
    flash('empty username!')
    return False

  cur = db.cursor(buffered=True)
  cur.execute(f"SELECT * FROM Login WHERE username='{userName}'")

  data = cur.fetchone()

  if data is None:
    flash('wrong password!')
    return False

  data = data[2]

  return sha256_crypt.verify(password, data)

@app.route('/login', methods=['GET'])
def display_login_page():
  if not session.get('logged_in'):
    return render_template('login.html')
  else:
    return redirect(url_for('home'))

@app.route('/login', methods=['POST'])
def receive_password():
  login = request.form

  account = check_password(login)

  if account:
    session['logged_in'] = True
    return redirect(url_for('home'))
  else:
    flash('wrong password!')
    return display_login_page()

@app.route('/logout')
def logout():
  session['logged_in'] = False
  return redirect(url_for('display_login_page'))

@app.route('/static/<path:path>')
def send_report(path):
  return send_from_directory('static', path)

@app.route('/variables', methods=['GET'])
def send_variables():
  cur = db.cursor(buffered=True)
  cur.execute(f"SELECT * FROM variables")

  data = str(cur.fetchall())

  return data

if __name__ == "__main__":
  app.secret_key = urandom(12)
  app.run(debug=False,host='0.0.0.0', port=80)
