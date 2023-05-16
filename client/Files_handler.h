#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <boost/filesystem.hpp>



#include <vector>
#include <sys/stat.h>
#include <boost/crc.hpp>
#include "boost/filesystem.hpp"
#include <boost/filesystem.hpp>
#include "boost/filesystem/fstream.hpp"
#include <boost/filesystem/fstream.hpp>
#include "Exceptions.h"


class Files_handler
{
public:
	static void write_to_file(std::string file_name, std::string content);
	static std::string* read_from_file(std::string file_name);
	static std::string read_file_by_path(std::string file_path);
	static long get_size_of_file(std::string filename);
	static uint32_t get_crc32(const std::string& my_string);
	static std::string get_file_name(std::string file_path);
	static void hexify(const unsigned char* buffer, unsigned int length);
	static void write_in_hex(std::string file_name, const unsigned char* buffer, unsigned int length);
	static unsigned char* id_back_to_hex(std::string hex_string);
	static unsigned char* get_id_from_meInfo();
};



