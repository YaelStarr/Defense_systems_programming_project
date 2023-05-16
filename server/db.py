import sqlite3
import SqlQueries
import datetime

CLIENTS_TABLE = {}

class db:
    def __init__(self, db_name: str):
        # open the db
        self.conn = sqlite3.connect(db_name)
        self.conn.text_factory = bytes
        self.cursor = self.conn.cursor()
        self.clients = list()
        self.files = list()
        self.load_database()

    def load_database(self):
        #global CLIENTS_TABLE
        try:
            self.cursor.execute(SqlQueries.GET_CLIENTS)
            self.clients = self.cursor.fetchall()

            for client in self.clients:
                current_client = [client[1], client[2], client[4]]
                CLIENTS_TABLE[client[0]] = current_client

        except:
            self.cursor.execute(SqlQueries.CREATE_CLIENTS)
            self.conn.commit()

        try:
            self.cursor.execute(SqlQueries.GET_FILES)
            self.files = self.cursor.fetchall()

        except:
            self.cursor.execute(SqlQueries.CREATE_FILES)
            self.conn.commit()

    def db_commit(self):
        self.conn.commit()

    def close_db(self):
        self.cursor.close()
        self.conn.close()

    def update_last_seen(self, request):
        self.cursor.execute(SqlQueries.UPDATE_LAST_SEEN_QUERY, [datetime.datetime.now(), request.client_id])
        self.db_commit()

    def do_query(self, query: str, params: list) -> list:
        # parameters = '[' + ','.join(params) + ']'

        try:
            self.cursor.execute(query, params)
            print("Succeeded in executing the query")
            return self.cursor.fetchall()

        except:
            print("failed in executing the query")
            return None

    def insert_update_query(self, query: str, params) -> None:
        try:
            self.cursor.execute(query, params)
            self.db_commit()
            print("Succeeded in executing the query")
        except:
            print("failed in executing the query")
