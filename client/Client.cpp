#include "Client.h"

Client::Client(std::string ip, std::string port, std::string client_name)
{
	this->ip = ip;
	this->port = port;
	this->client_name = client_name;

	unsigned char* fake_client_id = new unsigned char[16];
	Client::random_client_id(fake_client_id, 16);
	//this->client_id = fake_client_id;
	memcpy(client_id, fake_client_id, 16);
}

//Client::~Client() {
//	delete[] client_id;
//}

using boost::asio::ip::tcp;
std::string Client::send_data(const uint8_t* raw_buffer, const uint64_t size)
{
	const int max_length = 4096;
	boost::asio::io_context io_context;

	tcp::socket socket(io_context);
	tcp::resolver resolver(io_context);
	boost::asio::connect(socket, resolver.resolve(ip, port));
	boost::asio::write(socket, boost::asio::buffer(raw_buffer, size));
	boost::array<char, 1028> buffer;
	size_t len = socket.read_some(boost::asio::buffer(buffer, max_length));
	std::string response_data(buffer.begin(), len);

	socket.close();

	std::cout << "[DEBUG] Recieved response (size:" << std::to_string(len) << " )"<< std::endl;

	return response_data;
};


std::string Client::get_client_name()
{
	return client_name;
}

unsigned char* Client::get_client_id()
{
	return client_id;
}

void Client::set_client_id(unsigned char * client_id)
{
	std::memcpy(this->client_id, client_id, 17);
}


void Client::set_client_id(std::string client_id)
{
	unsigned char * client_id_ = Files_handler::id_back_to_hex(client_id);
	std::memcpy(this->client_id, client_id_, 17);
}

//generate fake id
void Client::random_client_id(unsigned char* buf, int size)
{
	srand(time(NULL));

	int low = 'A';
	int high = 'z';

	for (size_t i = 0; i < size; i++)
		buf[i] = low + std::rand() % (high - low + 1);
}
