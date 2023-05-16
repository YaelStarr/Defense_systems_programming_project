def save_file(path: str, data: bytearray):
    with open(path, 'wb') as file:
        file.write(data)


def unpad_binary(data: bytearray, padding: bytearray):
    return data.strip(padding)

def get_port():
    print("")