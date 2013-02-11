#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>

class Server {
public:
	Server(std::string const& _address, std::string const& _port = "6667", bool _ssl = false, std::string const& _password = "");
	~Server();

	std::string* getAddress();
	std::string* getPort();
	bool getSSL();
	std::string* getPassword();

private:
	std::string * address_;
	std::string * port_;
	std::string * password_;
	bool ssl_;


};

#endif