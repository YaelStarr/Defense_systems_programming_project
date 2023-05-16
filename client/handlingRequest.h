#pragma once

#include <string>
#include "FileRequestToServer.h"
#include "transfer_for_client.h"
#include "Files_handler.h"
#include "RSAWrapper.h"
#include "AESWrapper.h"
#include "Client.h"
#include "Exceptions.h"
#include "Base64Wrapper.h"

constexpr unsigned char CLIENT_VERSION = '3';
constexpr unsigned int RESPONSE_HEADERS_BYTES_SIZE = 7;
constexpr unsigned int RESPONSE_FILE_HEADERS_TOTAL_SIZE = RESPONSE_HEADERS_BYTES_SIZE + 275;
constexpr unsigned int NUMBER_OF_FILE_SEND_TRIES = 3;

constexpr unsigned int REGISTERATION_REQUEST_CODE = 1100;
constexpr unsigned int SEND_PK_REQUEST_CODE = 1101;
constexpr unsigned int RECONNECTING_REQUEST_CODE = 1102;
constexpr unsigned int SEND_FILE_REQUEST_CODE = 1103;
constexpr unsigned int VALID_CRC_REQUEST_CODE = 1104;
constexpr unsigned int INVALID_CRC_REQUEST_CODE = 1105;
constexpr unsigned int FATAL_INVALID_CRC_REQUEST_CODE = 1106;

constexpr unsigned int REGISTERATION_SUCCESSFUL_CODE = 2100;
constexpr unsigned int REGISTERATION_FAILED_CODE = 2101;
constexpr unsigned int RECEIVED_PK_SENT_AES = 2102;
constexpr unsigned int FILE_RECEIVED_AND_CRC = 2103;
constexpr unsigned int CONFIRMS_RECEIPT_MESSAGE = 2104;
constexpr unsigned int ALLOWING_RECONNECT = 2105;
constexpr unsigned int RECONNECT_REJECTED = 2106;
constexpr unsigned int GENERAL_ERROR = 2107;

class FileResponseFromServer
{

    unsigned char version;
    uint16_t code;
    uint32_t payload_size;
    char* payload;

    static int get_code(std::string request);

public:

    std::string* interprets_response(std::string response);
    static unsigned char* RegisterUser(Client tcp_client, transfer_for_client transfer, int code);
    static std::vector<std::string> Reconnecting(Client tcp_client, transfer_for_client transfer);
    static std::vector<std::string> SendPublicKey(Client tcp_client);
    static std::string DecryptAESKey(std::string encrypted_aes_key, std::string private_key);
    static std::string EncryptFileAES(std::string aes_key, std::string file_data);

    

    static bool SendFile(Client tcp_client, std::string aes_key);
    static void SendValidCRC(Client tcp_client, std::string filename);
    static void SendInvalidCRC(Client tcp_client, std::string filename);
    static void SendFatalInvalidCRC(Client tcp_client, std::string filename);

    static void random_client_id(unsigned char* buf, int size);
    static void add_null_terminating_byte(std::string str, char* buf);
    static std::string pad_string(const std::string& input);
};
