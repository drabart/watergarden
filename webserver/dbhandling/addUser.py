from passlib.hash import sha256_crypt
import mysql.connector as mariadb

def index():
  mariadb_connection = mariadb.connect(user='', password='', database='Login')

  username = input("give username: ")
  password = sha256_crypt.hash(input("give password: ")+"salt")

  cur = mariadb_connection.cursor()
  cur.execute('INSERT INTO Login (username, password) VALUES (%s, %s)', (username, password))
  mariadb_connection.commit()
  cur.close()

  return "New user added"

if __name__ == '__main__':
  index()
