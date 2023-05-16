import ast
import socket
import socket
import sqlite3
import os

from handlingRequests import request

# os.system("handlingRequests.py")
#
# FILE_PORT = "port.info.txt"
# SERVER_DB = "server.db"
# PORT = 1234
# MAXIMUM_PORT = 65535
# CLIENT_TABLE = []
DIRECTORY_PATH = "C:\\server_files"


directory_name = "C:\\server_files"
if not os.path.exists(DIRECTORY_PATH):
    os.makedirs(DIRECTORY_PATH, exist_ok=True)

# try:
#     with open(FILE_PORT, "r") as file:
#         PORT = file.read()
#
#         if not PORT.isnumeric() or (PORT < 0) or (PORT > MAXIMUM_PORT):
#             PORT = 1234
#             print('error- the port is invalid')
#
# except:
#     PORT = 1234
#     print('error -can not open %s file, the port is 1234' % FILE_PORT)
#
# # open the db
# conn = sqlite3.connect(r"server.db")
# cursor = conn.cursor()

# try:
#     cursor.execute("select * from clients")
#     CLIENT_TABLE = cursor.fetchall()
# except:
#     cursor.execute('''CREATE TABLE clients (ID VARCHAR(16), Name VARCHAR(255), PublicKey VARCHAR(160),LastSeen TIME,
#     AES VARCHAR(16) )''')
#     conn.commit()
#
# try:
#     cursor.execute("select * from files")
#     FILES_TABLE = cursor.fetchall()
# except:
#     cursor.execute('''CREATE TABLE files (ID VARCHAR(16), File_Name VARCHAR(255), Path_Name VARCHAR(255), Verified
#     BOOLEAN )''')
#     conn.commit()
# cursor.close()

# server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# server_socket.bind(("0.0.0.0", int(PORT)))
# server_socket.listen()
#
# print("Server is up and running")
(client_socket, client_address) = server_socket.accept()
print("Client connected")
with client_socket:
    while True:
        data = client_socket.recv(1024).decode()
        if not request(data) == 0:
            print("Client sent: " + data)
            if data == "Bye":
                data = " "
            if data == "Quit":
                print("closing client socket now...")
                client_socket.send("Bye".encode())
                break
            client_socket.send(data.encode())

client_socket.close()
server_socket.close()
cursor.close()
