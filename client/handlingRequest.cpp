#include "handlingRequest.h"


std::string* FileResponseFromServer::interprets_response(std::string response)
{
	version = response.at(0);
	return NULL;
}


void FileResponseFromServer::random_client_id(unsigned char* buf, int size)
{
	srand(time(NULL));

	int low = 'A';
	int high = 'z';

	for (size_t i = 0; i < size; i++)
		buf[i] = low + std::rand() % (high - low + 1);
}

void FileResponseFromServer::add_null_terminating_byte(std::string str, char* buf)
{
	std::vector<char> writable(str.begin(), str.end());
	writable.push_back('\0'); //Adding null-terminated byte

	std::memcpy(buf, &writable[0], str.length() + 1);

}

unsigned char* FileResponseFromServer::RegisterUser(Client tcp_client, transfer_for_client transfer, int code)
{
	try
	{
		std::cout << "[INFO] Starting with registering the user." << std::endl;
	
		FileRequestToServer request(tcp_client.get_client_id(), CLIENT_VERSION, code, transfer.get_client_name().length(), NULL);
		//generate Registration Request
		std::tuple<const uint8_t*, const uint64_t> tup = request.createRegistrationRequest(transfer.get_client_name());
		auto buff = std::get<0>(tup);
		auto size = std::get<1>(tup);

		auto response = tcp_client.send_data(buff, size);

		int code = get_code(response);
		std::cout << "[DEBUG] -------------THE CODE IS: ----------"<<code <<std::endl;

		if (code == REGISTERATION_FAILED_CODE)
		{ 
			delete buff;
			throw std::exception("User is already registered! (Code 2101)");
		}

		unsigned char* client_id = new unsigned char[16];
		std::memcpy(client_id, response.c_str() + 7, 16);

		delete buff;

		return client_id;
	}
	catch (const std::exception& ex)
	{
		throw ExceptionUserAlreadyRegistered(ex);
		std::cout << ex.what();
	}
}


std::vector<std::string> FileResponseFromServer::Reconnecting(Client tcp_client, transfer_for_client transfer)
{
	try
	{
		std::cout << "[INFO] Starting with reconnecting the user." << std::endl;

		FileRequestToServer request(tcp_client.get_client_id(), CLIENT_VERSION, RECONNECTING_REQUEST_CODE, transfer.get_client_name().length(), NULL);
		//generate Reconnection Request
		std::tuple<const uint8_t*, const uint64_t> tup = request.createRegistrationRequest(transfer.get_client_name());
		auto buff = std::get<0>(tup);
		auto size = std::get<1>(tup);

		auto response = tcp_client.send_data(buff, size);

		int code = get_code(response);
		std::cout << "[DEBUG] -------------THE CODE IS: ----------" << code << std::endl;
		if (code == RECONNECT_REJECTED)
			throw std::exception();
		int client_int_start = RESPONSE_HEADERS_BYTES_SIZE;
		int client_int_end = RESPONSE_HEADERS_BYTES_SIZE + 16;
		int aes_key_start = RESPONSE_HEADERS_BYTES_SIZE + 16;
		int aes_key_end = response.length() - aes_key_start;

		auto _client_id = response.substr(client_int_start, client_int_end - client_int_start);
		auto encrypted_aes_key = response.substr(aes_key_start, aes_key_end);

		std::vector<std::string> packed_encryped_and_id = { _client_id, encrypted_aes_key };

		delete buff;
		return packed_encryped_and_id;
	}
	catch (const std::exception& ex)
	{
		throw ExceptionUserAlreadyRegistered(ex);
	}
}


std::vector<std::string> FileResponseFromServer::SendPublicKey(Client tcp_client)
{

	try
	{

		std::cout << "[INFO] Starting RSA handshake with the server" << std::endl;

		RSAPrivateWrapper rsaprivate;
		std::string pubkey = rsaprivate.getPublicKey();
		std::string skey = rsaprivate.getPrivateKey();

		char* public_key = new char[pubkey.length()];
		std::memcpy(public_key, pubkey.c_str(), pubkey.length());
		FileRequestToServer request(tcp_client.get_client_id(), CLIENT_VERSION, SEND_PK_REQUEST_CODE, tcp_client.get_client_name().length() + pubkey.length(), NULL);
		std::tuple<const uint8_t*, const uint64_t> raw_request = request.createSendPublicKeyRequest(tcp_client.get_client_name(), public_key);//pubkey.c_str()<=>public_key

		auto buffer = std::get<0>(raw_request);
		auto buffer_size = std::get<1>(raw_request);

		auto response = tcp_client.send_data(buffer, buffer_size);

		int client_int_start = RESPONSE_HEADERS_BYTES_SIZE;
		int client_int_end = RESPONSE_HEADERS_BYTES_SIZE + 16;
		int aes_key_start = RESPONSE_HEADERS_BYTES_SIZE + 16;
		int aes_key_end = response.length() - aes_key_start;


		auto _client_id = response.substr(client_int_start, client_int_end - client_int_start);
		auto encrypted_aes_key = response.substr(aes_key_start, aes_key_end);

		/// write the private key to me.info
		Files_handler::write_to_file("me.info.txt", Base64Wrapper::encode(skey));

		std::vector<std::string> packed_encryped_and_sk = { skey, encrypted_aes_key };

		delete buffer;
		return packed_encryped_and_sk;
	}
	catch (const std::exception& ex)
	{
		
		std::cout << ex.what();
		throw ExceptionSendRSAPublicKey(ex);
	}
}




std::string FileResponseFromServer::DecryptAESKey(std::string encrypted_aes_key, std::string private_key)
{
	try {
		RSAPrivateWrapper rsapriv_other(private_key);

		std::string decrypted = rsapriv_other.decrypt(encrypted_aes_key);

		return decrypted;
	}
	catch (const std::exception& ex)
	{
		//std::cout << ex.what() << encrypted_aes_key << std::endl;
		throw std::exception(ex);
	}
}



std::string FileResponseFromServer::EncryptFileAES(std::string aes_key, std::string file_content)
{
	try {
		std::cout << "[INFO] Encrypting the file with received AES key" << std::endl;
		unsigned char* key = new unsigned char[AESWrapper::DEFAULT_KEYLENGTH];
		std::memcpy(key, aes_key.c_str(), AESWrapper::DEFAULT_KEYLENGTH);
		Files_handler::hexify(key, AESWrapper::DEFAULT_KEYLENGTH);
		AESWrapper aes(key, AESWrapper::DEFAULT_KEYLENGTH);
		int padd = 16 - file_content.length() % 16;
		for (int i = 0; i < padd; i++)
		{
			file_content += '\0';
		}
		std::string ciphertext = aes.encrypt(file_content.c_str(), file_content.length() + padd);
		
		return ciphertext;
	}
	catch (std::exception& e)
	{
		throw ExceptionSendingEncryptedFile(e);
	}
}


bool FileResponseFromServer::SendFile(Client tcp_client, std::string aes_key)
{
	try {
		std::cout << "[DEBUG INFO] Sending encrypted file" << std::endl;

		transfer_for_client transfer_client;
		std::string file_path = transfer_client.get_path();
		std::string filename = file_path.substr(file_path.find_last_of("/\\") + 1);

		std::string file_data = Files_handler::read_file_by_path(file_path);
		std::string encrypted_payload = FileResponseFromServer::EncryptFileAES(aes_key, file_data);

		FileRequestToServer request(tcp_client.get_client_id(), CLIENT_VERSION, SEND_FILE_REQUEST_CODE, 0, NULL);
		std::tuple<const uint8_t*, const uint64_t> raw_request = request.createSendFileRequest(encrypted_payload.length(), filename, encrypted_payload.c_str());

		auto buffer = std::get<0>(raw_request);
		auto buffer_size = std::get<1>(raw_request);

		uint32_t file_crc = Files_handler::get_crc32(file_data);
		std::cout << "[DEBUGE] ---------file crc is----------: "<< file_crc << std::endl;
		
		auto response = tcp_client.send_data(buffer, buffer_size);
		for(int i = 0; i < NUMBER_OF_FILE_SEND_TRIES; i++)
		{ 
			if (get_code(response) == GENERAL_ERROR)
			{
				response = tcp_client.send_data(buffer, buffer_size);
				std::cout << "error an with responded server " << std::endl;
			}
			else
			{
				break;
			}
		}
		


		int payload_start_index = RESPONSE_FILE_HEADERS_TOTAL_SIZE;
		int payload_length = response.length() - payload_start_index;

		auto crc_pointer = response.c_str() + payload_start_index;
		uint32_t data;
		std::memcpy((char*)&data, crc_pointer, sizeof(data));

		return file_crc == data;
	}


	catch (std::exception& e)
	{
		std::cout << e.what();
		throw ExceptionSendingEncryptedFile(e);
	}
}



void FileResponseFromServer::SendValidCRC(Client client, std::string filename)
{

	try
	{
		FileRequestToServer request = FileRequestToServer(client.get_client_id(), CLIENT_VERSION, VALID_CRC_REQUEST_CODE, filename.length(), nullptr);
		std::tuple<const uint8_t*, const uint64_t> tup = request.createRegistrationRequest(filename);
		auto buff = std::get<0>(tup);
		auto size = std::get<1>(tup);

		auto response = client.send_data(buff, size);

		int code = get_code(response);
		std::cout << "[DEBUG] -------------THE CODE IS: ----------" << code << std::endl;
		
		if (code != CONFIRMS_RECEIPT_MESSAGE)
		{
			//std::cout << "[DEBUG] Server error received code instead of code(Code 2104)" << code << std::endl;
			throw std::exception("Server error received code instead of code (Code 2104)");
		}
			
	}

	catch (std::exception& e)
	{
		throw ExceptionSendingFileIndication(e);
	}

}



void FileResponseFromServer::SendInvalidCRC(Client tcp_client, std::string filename)
{
	try {
		std::cout << "[DEBUG - ERROR] The server recieved invalid file (CRC is not correct)" << std::endl;

		FileRequestToServer request(tcp_client.get_client_id(), CLIENT_VERSION, INVALID_CRC_REQUEST_CODE, 0, nullptr);
		std::tuple<const uint8_t*, const uint64_t> curr_request = request.createRegistrationRequest(filename);

		auto buffer = std::get<0>(curr_request);
		auto buffer_size = std::get<1>(curr_request);

		tcp_client.send_data(buffer, buffer_size);

	}

	catch (std::exception& e)
	{
		throw ExceptionSendingFileIndication(e);
	}

}
void FileResponseFromServer::SendFatalInvalidCRC(Client tcp_client, std::string filename)
{
	try {
		std::cout << "[FATAL] Terminating session, file is invalid after 3 tries." << std::endl;

		FileRequestToServer request(tcp_client.get_client_id(), CLIENT_VERSION, FATAL_INVALID_CRC_REQUEST_CODE, 0, nullptr);
		std::tuple<const uint8_t*, const uint64_t> curr_request = request.createRegistrationRequest(filename);

		auto buffer = std::get<0>(curr_request);
		auto buffer_size = std::get<1>(curr_request);

		tcp_client.send_data(buffer, buffer_size);
	}


	catch (std::exception& e)
	{
		throw ExceptionSendingFileIndication(e);
	}
}


int FileResponseFromServer::get_code(std::string response)
{
	if (response.length() < 3)
		return 0;
	int num = response.at(2);
	num = num << 8;
	num += response.at(1);
	return num;
}



std::string FileResponseFromServer:: pad_string(const std::string& input) {
	int padding_size = 16 - (input.length() % 16);
	char padding_byte = static_cast<char>(padding_size);
	std::string output = input;
	output.append(padding_size, padding_byte);
	return output;
}