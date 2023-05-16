#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include "Files_handler.h"





class Client
{
private:
	std::string ip;
	std::string port;
	std::string client_name;
	unsigned char *client_id = new unsigned char[17];

public:
	Client(std::string, std::string, std::string);
	//~Client();
	std::string send_data(const uint8_t* buf, const uint64_t size);
	std::string get_client_name();
	unsigned char* get_client_id();
	void set_client_id(unsigned char* client_id);
	void set_client_id(std::string client_id);
	static void random_client_id(unsigned char* buf, int size);
};


