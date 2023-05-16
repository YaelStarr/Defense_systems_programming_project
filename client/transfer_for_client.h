#pragma once
#include <iostream>
#include <fstream>
#include <string>

//this file contain the data recived from transfer.info file

class transfer_for_client
{
private:
	std::string ip;
	std::string port;
	std::string client_name;
	std::string path;
	std::string FILE_NAME = "transfer.info.txt";

public:
	transfer_for_client();
	std::string get_ip();
	std::string get_port();
	std::string get_client_name();
	std::string get_path();
};

