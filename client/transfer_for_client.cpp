#include "transfer_for_client.h"


transfer_for_client::transfer_for_client() {
	std::string content;
	int index;

	std::ifstream ReadFile(FILE_NAME);
	getline(ReadFile, content);

	index = content.find(":");
	ip = content.substr(0, index);
	port = content.substr(index + 1);

	getline(ReadFile, client_name);
	getline(ReadFile, path);
}

std::string transfer_for_client::get_ip()
{
	return ip;
}

std::string transfer_for_client::get_port()
{
	return port;
}

std::string transfer_for_client::get_client_name()
{
	return client_name;
}
std::string transfer_for_client::get_path()
{
	return path;
}




