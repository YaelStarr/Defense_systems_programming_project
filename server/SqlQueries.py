# clients queries
INSERT_NEW_CLIENT_QUERY = 'INSERT INTO clients (ID, Name, LastSeen) VALUES (?, ?, ?)'
UPDATE_LAST_SEEN_QUERY = 'UPDATE clients SET LastSeen = ? WHERE ID=?'
UPDATE_PUBLIC_KEY_QUERY = "UPDATE clients SET PublicKey = ? WHERE ID = ?"
UPDATE_AES_QUERY = "UPDATE clients SET AES = ? WHERE ID = ?"
GET_CLIENT_BY_NAME_QUERY = "SELECT * FROM clients WHERE Name = ?"
GET_AES_BY_NAME_AND_ID_QUERY = "select AES from clients where ID=? and Name = ?"
GET_AES_BY_ID_QUERY = "select AES from clients where ID=?"
GET_CLIENTS = "SELECT * FROM clients"
CREATE_CLIENTS = '''CREATE TABLE clients (ID VARCHAR(16), Name VARCHAR(255), PublicKey VARCHAR(160),
                    LastSeen TIME, AES VARCHAR(16) )'''

# files queries
INSERT_NEW_FILE_QUERY = "INSERT INTO files (ID, File_Name, Path_Name, Verified) VALUES (?, ?, ?, ?)"
UPDATE_VERIFIED = "UPDATE files SET Verified = True WHERE ID = ? AND File_Name = ?"
GET_FILES = "SELECT * FROM files"
CREATE_FILES = '''CREATE TABLE files (ID VARCHAR(16), File_Name VARCHAR(255), Path_Name VARCHAR(255), 
                    Verified BOOLEAN )'''
