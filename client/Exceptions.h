#pragma once
#include <exception>
#include <string>
#include <iostream>


class ExceptionUserAlreadyRegistered : public std::exception {
private:
	std::string msg;
public:
	ExceptionUserAlreadyRegistered(const std::exception e);
	virtual const char* what() const throw();
};


class ExceptionInMeInfo : public std::exception {
private:
	std::string msg;
public:
	ExceptionInMeInfo(const std::exception e);
	virtual const char* what() const throw();
};


class ErrorReconnectRejected : public std::exception {
private:
	std::string msg;
public:
	ErrorReconnectRejected(const std::exception e);
	virtual const char* what() const throw();
};


class ExceptionSendingFileIndication : public std::exception {
private:
	std::string msg;
public:
	ExceptionSendingFileIndication(const std::exception e);
	virtual const char* what() const throw();
};


class ExceptionSendingEncryptedFile : public std::exception {
private:
	std::string msg;
public:
	ExceptionSendingEncryptedFile(const std::exception e);
	virtual const char* what() const throw();
};


class ExceptionSendRSAPublicKey : public std::exception {
private:
	std::string msg;
public:
	ExceptionSendRSAPublicKey(const std::exception e);
	virtual const char* what() const throw();
};


