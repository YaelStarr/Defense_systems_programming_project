import struct


class ServerRequest:

    def __init__(self):
        self.client_id = 0
        self.version = 0
        self.code = 0
        self.payload_size = 0
        self.payload = ""

    def parse_binary_request(self, request):
        END_OF_BASE_REQUEST = 23

        try:
            base_request = request[:END_OF_BASE_REQUEST]
            format = '<'  # Little endian annotation
            format += '16s'
            format += 'c'
            format += 'H'
            format += 'I'

            self.client_id, self.version, self.code, self.payload_size = struct.unpack(format, base_request)
            length = int(self.payload_size)
            if(length > 0):
                self.payload = request[END_OF_BASE_REQUEST: END_OF_BASE_REQUEST + length]

        except:
             # do something
            exit(1)

