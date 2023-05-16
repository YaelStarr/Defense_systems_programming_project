#include "Exceptions.h"

ExceptionUserAlreadyRegistered::ExceptionUserAlreadyRegistered(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "The registration process failed. The user is already registered. Try the reconnect request. \n";
	formatted_msg += e.what();
	msg = formatted_msg.c_str();
}

const char* ExceptionUserAlreadyRegistered::what() const throw() {
	std::cout << msg;
	return msg.c_str();
}




ExceptionInMeInfo::ExceptionInMeInfo(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "Can't open me.info file. \n";
	formatted_msg += e.what();
	msg = formatted_msg.c_str();
}

const char* ExceptionInMeInfo::what() const throw() {
	std::cout << msg;
	return msg.c_str();
}



ErrorReconnectRejected::ErrorReconnectRejected(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "The request to reconnect has been rejected. \n";
	formatted_msg += e.what();
	msg = formatted_msg.c_str();

}

const char* ErrorReconnectRejected::what() const throw() {
	std::cout << msg;
	return msg.c_str();
}


ExceptionSendingFileIndication::ExceptionSendingFileIndication(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "Exception in sending file indication. \n";
	formatted_msg += e.what();
	msg = formatted_msg.c_str();

}

const char* ExceptionSendingFileIndication::what() const throw() {
	std::cout << msg;
	return msg.c_str();
}


ExceptionSendingEncryptedFile::ExceptionSendingEncryptedFile(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "Exception in sending file indication. \n";
	formatted_msg += e.what();
	msg = formatted_msg.c_str();

}

const char* ExceptionSendingEncryptedFile::what() const throw() {
	std::cout << msg;
	return msg.c_str();
}



ExceptionSendRSAPublicKey::ExceptionSendRSAPublicKey(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "Exception in send RSA publicKey. \n";
	formatted_msg += e.what();
	msg = formatted_msg.c_str();

}

const char* ExceptionSendRSAPublicKey::what() const throw() {
	std::cout << msg;
	return msg.c_str();
}



