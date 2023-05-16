import struct


class ResponseFromServer:
    def __init__(self, version, code, payload_size, payload=None):
        self.version = version
        self.code = code
        self.payload_size = payload_size
        self.payload = payload

    def parse_binary_response(self):

        try:

            format = '<'  # Little endian annotation
            format += 'c'  # version
            format += 'H'  # code
            format += 'I'  # unsigned payload size

            if self.payload:
                format += f'{self.payload_size}s'
                if type(self.payload) is str:
                    self.payload = self.payload.encode()


                print('1 - there is payload')
                return struct.pack(format, self.version.to_bytes(1, 'little'), self.code, self.payload_size, self.payload)
            print('2 - no payload')
            return struct.pack(format, self.version.to_bytes(1, 'little'), self.code, self.payload_size)

        except Exception as e:
            # do something
            print(e)
