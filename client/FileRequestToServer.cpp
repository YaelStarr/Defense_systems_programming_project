#include "FileRequestToServer.h"

//constractor
FileRequestToServer::FileRequestToServer(unsigned char* client_id, unsigned char version, uint16_t code, uint32_t payload_size, char* payload)
{
	this->client_id = client_id;
	this->version = version;
	this->code = code;
	this->payload_size = payload_size;
	this->payload = payload;

}



//All requests that contain content of size 255
std::tuple<const uint8_t*, const uint64_t> FileRequestToServer::createRegistrationRequest(std::string client_name)
{
	
#pragma pack(push, 1)

	struct BasicRequest {
		unsigned char client_id[16];
		unsigned char version;
		uint16_t code;
		uint32_t payload_size;
	};

	struct RegisterRequest
	{
		BasicRequest basic_req;
		char payload[255];
	};

	struct RegisterRequest request;

	std::memcpy(request.basic_req.client_id, client_id, 16);
	request.basic_req.version = version;
	request.basic_req.code = code;
	request.basic_req.payload_size = payload_size;

	std::memcpy(request.payload, client_name.c_str(), payload_size);
	request.payload[payload_size] = '\0';

	const uint8_t* temp_buffer = reinterpret_cast<const uint8_t*>(&request);

	uint8_t* raw_buffer = new uint8_t[sizeof(request)];

	std::memcpy(raw_buffer, temp_buffer, sizeof(request));

	const uint8_t* final_buffer = raw_buffer;

	return std::make_tuple(final_buffer, sizeof(request));

#pragma pack(pop)
}




std::tuple<const uint8_t*, const uint64_t> FileRequestToServer::createSendPublicKeyRequest(std::string client_name, char* public_key)
{
#	pragma pack(push, 1)

	struct BasicRequest {
		unsigned char client_id[16];
		unsigned char version;
		uint16_t code;
		uint32_t payload_size;
	};

	struct SendPublicKeyRequest
	{
		BasicRequest basic_req;
		char client_name[255];
		char payload[160];
	};

	struct SendPublicKeyRequest request;


	client_name += '\0';
	std::memcpy(request.basic_req.client_id, client_id, 16);
	request.basic_req.version = version;
	request.basic_req.code = code;
	request.basic_req.payload_size = sizeof(request) - sizeof(request.basic_req);

	std::memcpy(request.client_name, client_name.c_str(), 255);		
	std::memcpy(request.payload, public_key, 160);			


	const uint8_t* temp_buffer = reinterpret_cast<const uint8_t*>(&request);

	uint8_t* raw_buffer = new uint8_t[sizeof(request)];

	std::memcpy(raw_buffer, temp_buffer, sizeof(request));

	const uint8_t* final_buffer = raw_buffer;

	return std::make_tuple(final_buffer, sizeof(request));

#pragma pack(pop)
}



std::tuple<const uint8_t*, const uint64_t> FileRequestToServer::createSendFileRequest(size_t file_size, std::string file_name, std::string encrypted_file_content)
{
#    pragma pack(push, 1)

	struct RawRequest {
		unsigned char client_id[16];
		unsigned char version;
		uint16_t code;
		uint32_t payload_size;
	};

	struct SendAESEncryptedFile
	{
		RawRequest raw_req;
		uint32_t file_size;
		char file_name[255];
		//char* message_content;
	};

	struct SendAESEncryptedFile req;

	file_name = file_name + '\0';
	//encrypted_file_content = encrypted_file_content + '\0';
	std::memcpy(req.raw_req.client_id, client_id, 16);
	req.raw_req.version = version;
	req.raw_req.code = code;
	req.file_size = file_size;


	std::memcpy(req.file_name, file_name.c_str(), 255);
	req.raw_req.payload_size = sizeof(req.file_size) + sizeof(req.file_name) + encrypted_file_content.length();
	
	const uint8_t* temp_buffer = reinterpret_cast<const uint8_t*>(&req);

	size_t total_buffer_size = sizeof(req) + req.raw_req.payload_size;
	uint8_t* raw_buffer = new uint8_t[total_buffer_size];

	std::memcpy(raw_buffer, temp_buffer, sizeof(req));
	std::memcpy(raw_buffer + sizeof(req), encrypted_file_content.c_str(), encrypted_file_content.length());

	const uint8_t* final_buffer = raw_buffer;

	return std::make_tuple(final_buffer, total_buffer_size);

#pragma pack(pop)
}





std::tuple<const uint8_t*, const uint64_t> FileRequestToServer::createValidationFileIndication(std::string file_name)
{
#	pragma pack(push, 1)

	struct baseRequest {
		unsigned char client_id[16];
		unsigned char version;
		uint16_t code;
		uint32_t payload_size;
	};

	struct ValidFileRequest
	{
		baseRequest base_req;
		//char client_id[16];
		char file_name[255];
	};

	struct ValidFileRequest req;


	std::memcpy(req.base_req.client_id, client_id, 16);
	req.base_req.version = version;
	req.base_req.code = code;

	req.base_req.payload_size = sizeof(req.file_name);
	std::memcpy(req.file_name, file_name.c_str(), file_name.length());
	const uint8_t* buffer = reinterpret_cast<const uint8_t*>(&req);
	return std::make_tuple(buffer, sizeof(req));

#pragma pack(pop)
}











