#pragma once
#include <stdint.h>
#include <tuple>
#include <string>
//This file contains the structures of the requests to the server

class FileRequestToServer
{
private:
    unsigned char* client_id;
    unsigned char version;
    uint16_t code;
    uint32_t payload_size;
    char* payload;

public:
    FileRequestToServer(unsigned char*, unsigned char, uint16_t, uint32_t, char*);
    std::tuple<const uint8_t*, const uint64_t> createRegistrationRequest(std::string);
    std::tuple<const uint8_t*, const uint64_t> createSendPublicKeyRequest(std::string, char*);
    std::tuple<const uint8_t*, const uint64_t> createSendFileRequest(size_t, std::string, std::string encrypted_file_content);
    std::tuple<const uint8_t*, const uint64_t> createValidationFileIndication(std::string file_name);
};


