#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <string>

#include "Parser.h"
#include "Server.h"
#include "User.h"
#include "debug.h"

class User;

class Connection : public virtual Parser {
public:
	Connection(User* _user, Server* _server);
	~Connection();
	void start();
    void stop();

	std::string parse(const std::string & _message);

private:
	bool connected_;
	int socket_;
	std::string * in_;

	Server * server_;
	User * user_;

	bool sendData(std::string* _msg);
	bool sendData(std::string _msg);
};

#endif