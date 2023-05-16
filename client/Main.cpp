#include "Main.h"

//********************************************
//* Course: Defensive Systems Programming.
//* Writtenand submitted by Yael Starr, 324864776
//* client side
//
//Explanation of how the program works:
//The client will move from the main function to a function responsible for the flow of requestsand responses from the server.
//For each request from the server there are 2 main steps :
//1. The request handler who sees the big picture and is actually responsible for the communication with the server.
//2. The structure of the request - what is finally sent to the server
//The rest of the files are ancillary files
//********************************************

int main()
{
	try {
			transfer_for_client transfer;
			Client client_tcp(transfer.get_ip(), transfer.get_port(), transfer.get_client_name());
			try {
				ClientManager::interaction_between_server_client(client_tcp, transfer);
			}
			catch (const std::exception& ex) {
				std::cout << ex.what();
			}
		}

		catch (const std::exception& e) {
			std::cout << "exception in main" << std::endl;
		}
		return 0;
	}


