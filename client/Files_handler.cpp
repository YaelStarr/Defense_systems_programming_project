#include "Files_handler.h"

void Files_handler::write_to_file(std::string file_name, std::string content)
{
	std::fstream file;
	file.open(file_name, std::ios::app);
	if (!file)
	{
		std::cout << "exception in me.info";
	}
	else{
		file << content;
		file.close();
	}
	
}


std::string* Files_handler::read_from_file(std::string file_name)
{
	std::string line;
	std::string* content = new std::string[3];
	int i ;
	for (i = 0; i < 3; i++)
	{
		content[i] = "";
	}

	int index = 0;
	std::ifstream ReadFile(file_name);
	while (ReadFile && getline(ReadFile, line) && index <3)
	{
		if (index < 2)
		{
			line += '\0';
			content[index++] = line;
		}
		else
		{
			content[index] += line;
		}
		
	}
	content[index] += '\0';
	return content;
}


std::string Files_handler::read_file_by_path(std::string file_path)
{

	std::ifstream infile(file_path);

	infile.seekg(0, std::ios::end);
	size_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	char* buffer = new char[length];

	infile.read(buffer, length);

	std::string final_buffer(buffer, length);

	delete buffer;
	return final_buffer;

}

long Files_handler::get_size_of_file(std::string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

uint32_t Files_handler::get_crc32(const std::string& my_string)
{
	boost::crc_32_type result;
	result.process_bytes(my_string.data(), my_string.length());
	return result.checksum();
}


std::string Files_handler::get_file_name(std::string file_path)
{
	std::string filename = file_path.substr(file_path.find_last_of("/\\") + 1);
	return filename;
}

void Files_handler::hexify(const unsigned char* buffer, unsigned int length)
{
	std::ios::fmtflags f(std::cout.flags());
	std::cout << std::hex;
	for (size_t i = 0; i < length; i++)
		std::cout << std::setfill('0') << std::setw(2) << (0xFF & buffer[i]) << (((i + 1) % 16 == 0) ? "\n" : " ");
	std::cout << std::endl;
	std::cout.flags(f);
}


void Files_handler::write_in_hex(std::string file_name, const unsigned char* buffer, unsigned int length)
{


	std::ios::fmtflags f(std::cout.flags());
	std::cout << std::hex;

	std::ofstream fout(file_name, std::ios::app);
	std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(fout.rdbuf()); //redirect std::cout to out.txt!

	std::cout << "\n";

	for (size_t i = 0; i < length; i++)
		std::cout << std::setfill('0') << std::setw(2) << (0xFF & buffer[i]) << (((i + 1) % 16 == 0) ? "\n" : "");
	std::cout.flags(f);
	std::cout.rdbuf(coutbuf); //reset to standard output again
}


unsigned char * Files_handler::id_back_to_hex(std::string hex_string)
{
	std::string output_string;

	for (int i = 0; i < hex_string.length()-1; i += 2) {
		std::string byte_string = hex_string.substr(i, 2);
		int byte = std::stoi(byte_string, nullptr, 16);
		output_string += static_cast<char>(byte);
	}
	const char* char_array = output_string.c_str();
	unsigned char uchar_array[17];

	std::copy(char_array, char_array + output_string.size(), uchar_array);
	uchar_array[16] = '\0';
	return uchar_array;
}

unsigned char* Files_handler::get_id_from_meInfo()
{
	std::string* content = Files_handler::read_from_file("me.info.txt");
	std::string str_client_id = content[1];
	return Files_handler::id_back_to_hex(str_client_id);
}