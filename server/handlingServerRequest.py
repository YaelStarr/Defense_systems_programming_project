import zlib

import Request
import Response
import sqlite3
import datetime
import uuid
import crypto
from crypto import get_crc_sum
import struct
# import server
from File_handler import save_file, unpad_binary
from Crypto.Cipher import AES
import SqlQueries
import db
from crypto import *
from base64 import b64encode
from db import CLIENTS_TABLE
VERSION = 3



def requestFromServer(request, database):
    print("Received a request from the client!!")
    current_request = Request.ServerRequest()
    current_request.parse_binary_request(request)

    COMMAND_DICTIONARY = {1100: registration, 1101: sendPublicKey, 1102: reconnecting, 1103: sendFile,
                          1104: corectCRC, 1105: sendAgainCRC, 1106: wrongCRCFinish}

    current_response = None
    if current_request.code in COMMAND_DICTIONARY:
        print("the request have code: " + str(current_request.code))
        current_response = COMMAND_DICTIONARY[current_request.code](current_request, database)

    else:
        return Response.ResponseFromServer(VERSION, 2107, 0).parse_binary_response()
        exit(1)

    return current_response


def registration(request, database):
    client_name = request.payload[:request.payload_size]
    client_exists: bool = False
    for client in db.CLIENTS_TABLE.values():
        print(client[0])
        print(client_name)
        if client[0] == client_name:
            client_exists = True
            break
    if client_exists:
        return Response.ResponseFromServer(VERSION, 2101, 0).parse_binary_response()

    else:
        user_uuid = uuid.uuid1()
        print(str(user_uuid))
        db.CLIENTS_TABLE[user_uuid.bytes] = [request.payload[:request.payload.find(b'\0')], None, None]
        database.insert_update_query(SqlQueries.INSERT_NEW_CLIENT_QUERY,
                                     [user_uuid.bytes, request.payload[:request.payload.find(b'\0')],
                                      datetime.datetime.now()])
        return Response.ResponseFromServer(VERSION, 2100, 16, user_uuid.bytes).parse_binary_response()


def sendPublicKey(request, database):
    database.update_last_seen(request)

    client_user_name: bytearray = request.payload[:255]
    client_name: str = client_user_name[0:client_user_name.find(b'\0')]
    public_key = request.payload[255:]
    client_exists: bool = False
    for client_id in db.CLIENTS_TABLE.keys():
        print(client_id)
        print(request.client_id)
        if client_id == request.client_id:
            client_exists = True
            break
    if not client_exists:
        return Response.ResponseFromServer(VERSION, 2107, 0).parse_binary_response()

    aes_key = random_aes_key()
    database.insert_update_query(SqlQueries.UPDATE_PUBLIC_KEY_QUERY, [public_key, request.client_id])

    db.CLIENTS_TABLE.get(request.client_id)[1] = public_key
    db.CLIENTS_TABLE.get(request.client_id)[2] = aes_key

    database.insert_update_query(SqlQueries.UPDATE_AES_QUERY, [aes_key, request.client_id])
    try:
        encrypted_aes_key = rsa_encryption(data=aes_key, public_key=public_key)
    except Exception as ex:
        print(ex)
        return Response.ResponseFromServer(VERSION, 2107, 0).parse_binary_response()

    print("[Debug] eas key: " + str(aes_key.hex()))
    format = f'<16s{len(encrypted_aes_key)}s'
    payload = struct.pack(format, request.client_id, encrypted_aes_key)
    return Response.ResponseFromServer(VERSION, 2102, len(payload), payload).parse_binary_response()


def reconnecting(request, database):
    database.update_last_seen(request)
    client_name = request.payload

    client_exists: bool = False
    for client_id in db.CLIENTS_TABLE.keys():
        print("client id in rhe list: " + str(client_id))
        print("client id in rhe list: " + str(request.client_id))
        if client_id == request.client_id:
            client_exists = True
            break

    if not client_exists:
        return Response.ResponseFromServer(VERSION, 2106, 0).parse_binary_response()

    try:
        public_key = db.CLIENTS_TABLE.get(request.client_id)[1]
        print(public_key)
        aes_key = db.CLIENTS_TABLE.get(request.client_id)[2]

        if public_key is None or aes_key is None:
            raise Exception
        encrypted_aes_key = rsa_encryption(data=aes_key, public_key=public_key)

    except Exception:
        return Response.ResponseFromServer(VERSION, 2106, 0).parse_binary_response()

    print("[Debug] eas key: " + str(aes_key.hex()))
    format = f'<16s{len(encrypted_aes_key)}s'
    payload = struct.pack(format, request.client_id, encrypted_aes_key)
    return Response.ResponseFromServer(VERSION, 2105, len(payload), payload).parse_binary_response()










def sendFile(request, database):
    database.update_last_seen(request)

    total_payload_headers_size = 259
    content_size, file_name = struct.unpack('<I255s', request.payload[:total_payload_headers_size])

    message_content = request.payload[total_payload_headers_size:]

    file_name = file_name[0:file_name.find(b'\0')]
    database.insert_update_query(SqlQueries.INSERT_NEW_FILE_QUERY, [request.client_id, file_name, "C:\server_files", False])

    aes_key = db.CLIENTS_TABLE.get(request.client_id)[2]

    try:
        decrypted_file = crypto.aes_decryption(aes_key=aes_key, data=message_content)
        decrypted_file = decrypted_file[0:decrypted_file.find(b'\0')] # I add 23.3.23
    except Exception as ex:
        print(ex)
        return Response.ResponseFromServer(VERSION, 2107, 0).parse_binary_response()

    save_file(path="C:\\server_files" + '\\' + str(file_name), data=decrypted_file)
    ##crc
    checkSum = crypto.get_crc_sum(decrypted_file)
    print("[DEBUG] check sum is: " + str(checkSum))

    format = '<16sI255sI'
    payload = struct.pack(format, request.client_id, content_size, file_name, checkSum)
    return Response.ResponseFromServer(VERSION, 2103, 279, payload).parse_binary_response()


def corectCRC(request, database):
    database.update_last_seen(request)

    client_name: bytearray = request.payload[:255]
    client_name: str = client_name[0:client_name.find(b'\0')].decode()
    database.insert_update_query(SqlQueries.UPDATE_VERIFIED, [request.client_id, client_name])
    return Response.ResponseFromServer(VERSION, 2104, 16, request.client_id).parse_binary_response()


def sendAgainCRC(request, database):
    database.update_last_seen(request)
    return Response.ResponseFromServer(VERSION, 2104, 0, None).parse_binary_response()


def wrongCRCFinish(request, database):
    database.update_last_seen(request)
    return Response.ResponseFromServer(VERSION, 2104, 0, None).parse_binary_response()
