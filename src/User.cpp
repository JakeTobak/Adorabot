#include "User.h"
#include <string>

using namespace std;
 
User::User(std::string _nick) {
    this->setNick(_nick);
    this->setIdent(_nick);
    this->setRealname(_nick);
}
 
User::User(std::string _nick, std::string _ident) {
    this->setNick(_nick);
    this->setIdent(_ident);
    this->setRealname(_ident);
}

User::User(std::string _nick, std::string _ident, std::string _realname) {
    this->setNick(_nick);
    this->setIdent(_ident);
    this->setRealname(_realname);
}

User::~User() {
    delete nick_;
    delete ident_;
    delete realname_;
    delete hostmask_;
}

void User::setNick(std::string _nick) {
	nick_ = new string(_nick);
}

void User::setIdent(std::string _ident) {
	ident_ = new string(_ident);
}

void User::setRealname(std::string _realname) {
	realname_ = new string(_realname);
}

void User::setHostmask(std::string _hostmask) {
	hostmask_ = new string(_hostmask);
}

string* User::getNick() {
	return nick_;
}

string* User::getIdent() {
	return ident_;
}

string* User::getRealname() {
	return realname_;
}

string* User::getHostmask() {
	return hostmask_;
}

