import os
import threading
import db
import socket
from handlingServerRequest import requestFromServer

FILE_PORT = "port.info.txt"
SERVER_DB = "server.db"
PORT = 1234
MAXIMUM_PORT = 65535
DIRECTORY_PATH = "C:\\server_files"


def get_port() -> str:
    try:
        file = open(FILE_PORT)
        port = (file.read())
        file.close()
        if not port.isnumeric() or (int(port) < 0) or (int(port) > MAXIMUM_PORT):
            port = 1234
            print('error- the port is invalid')

    except:
        port = 1234
        print('error -can not open %s file, the port is 1234' % FILE_PORT)

    return port

def handle_client(client_socket):
    print("Client connected")
    database = db.db(SERVER_DB)
    req = bytearray()
    batch = 20000
    packet = client_socket.recv(batch)
    req.extend(packet)
    response = requestFromServer(req, database)
    client_socket.send(response)
    # Close the connection
    client_socket.close()
    database.close_db()

def listen(port: str, database: db):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(("0.0.0.0", int(port)))
    server_socket.listen()
    print("Server is up and running")

    while True:
        client_socket, client_address = server_socket.accept()

        print(f"Received connection from {client_address}")
        # Create a new thread to handle the client
        client_thread = threading.Thread(target=handle_client, args=(client_socket,))
        client_thread.start()

    client_socket.close()
    server_socket.close()


def main():
    try:
        if not os.path.exists(DIRECTORY_PATH):
            os.makedirs(DIRECTORY_PATH, exist_ok=True)

        database = db.db(SERVER_DB)
        database.load_database()
        database.close_db()
        port = get_port()
        listen(port, database)
    except:
        exit(1)


if __name__ == "__main__":
    main()
