#include "ClientManager.h"

void ClientManager::interaction_between_server_client(Client client, transfer_for_client transfer)
{
	try {
		std::string decrypted_aes_key;
		std::string encrypted_aes_key;
		std::string filename = Files_handler::get_file_name(transfer.get_path());
		bool exists_meInfo = ClientManager::exists_meInfo();
		if (!exists_meInfo) //RegisterUser
		{
			Files_handler().write_to_file("me.info.txt", transfer.get_client_name());
			client.set_client_id(FileResponseFromServer::RegisterUser(client, transfer, 1100));
			//Enter a unique ID into the me.info
			Files_handler::write_in_hex("me.info.txt", client.get_client_id(), ID_LENGTH);
			std::cout << "Registered succesfully! \n" << std::endl;

			auto aes_key_to_decrypt = FileResponseFromServer::SendPublicKey(client);
			std::string private_key = aes_key_to_decrypt.front();
			encrypted_aes_key = aes_key_to_decrypt.back();
			decrypted_aes_key = FileResponseFromServer::DecryptAESKey(encrypted_aes_key, private_key);
		} //end if

		else //Reconnecting
		{
			//get private key from me info
			std::string private_key;
			std::string client_id;
			try {
				private_key = Files_handler::read_from_file("me.info.txt")[2];
				client_id = Files_handler::read_from_file("me.info.txt")[1];
			}
			catch (const std::exception& ex)
			{
				throw ExceptionInMeInfo(ex);
			}

			client.set_client_id(client_id);
			private_key = Base64Wrapper::decode(private_key);
			try {
				auto id_and_aes_key_to_decrypt = FileResponseFromServer::Reconnecting(client, transfer);
				encrypted_aes_key = id_and_aes_key_to_decrypt.back();
			}
			catch (const std::exception& ex)
			{
				throw ErrorReconnectRejected(ex);
				std::cout << ex.what();
			}

			decrypted_aes_key = FileResponseFromServer::DecryptAESKey(encrypted_aes_key, private_key);

		}
		bool validated_file = false;
		for (size_t i = 0; i < NUMBER_OF_FILE_SEND_TRIES; i++)
		{
			validated_file = FileResponseFromServer::SendFile(client, decrypted_aes_key);
			if (validated_file)
			{
				std::cout << "File recieved succesfully! \n" << std::endl;
				FileResponseFromServer::SendValidCRC(client, filename);
				break;
			}

			else
				FileResponseFromServer::SendInvalidCRC(client, filename);
		}

		if (!validated_file)
			FileResponseFromServer::SendFatalInvalidCRC(client, filename);

		//delete client_id;

	}

	catch (const ExceptionUserAlreadyRegistered& ex)
	{
		FileResponseFromServer::RegisterUser(client, transfer, 1101);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
	}
	

}

bool ClientManager::exists_meInfo()
{
	std::ifstream ifile;
	ifile.open("me.info.txt");
	if (ifile)
	{
		ifile.close();
		return true;
	}
	return false;
}



