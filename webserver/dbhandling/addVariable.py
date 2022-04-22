from passlib.hash import sha256_crypt
import mysql.connector as mariadb

def index():
  mariadb_connection = mariadb.connect(user='', password='', database='Login')

  name = input("give variable name: ")
  value = input("give variable value: ")
  type = input("give variable type: ")

  cur = mariadb_connection.cursor()
  cur.execute('INSERT INTO variables (name, type, value) VALUES (%s, %s, %s)', (name, type, value))
  mariadb_connection.commit()
  cur.close()

  return "New variable added"

if __name__ == '__main__':
  index()
