#include "Server.h"

#include <string>

Server::Server(const std::string & _address, const std::string & _port, bool _ssl, std::string const& _password) {
	address_ = new std::string(_address);
	port_ = new std::string(_port);
	password_ = new std::string(_password);
	ssl_ = _ssl;
}

Server::~Server() {
	delete address_;
	delete port_;
	delete password_;
}

std::string* Server::getAddress() {
	return address_;
}

std::string* Server::getPort() {
	return port_;
}

bool Server::getSSL() {
	return ssl_;
}

std::string* Server::getPassword() {
	return password_;
}