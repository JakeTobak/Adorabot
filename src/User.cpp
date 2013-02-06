#include "User.h"
#include <string>
#include <string.h>

using namespace std;
 
User::User(char * _nick) {
    this->setNick(_nick);
    this->setIdent(_nick);
    this->setRealname(_nick);
}
 
User::User(char * _nick, char* _ident) {
    this->setNick(_nick);
    this->setIdent(_ident);
    this->setRealname(_nick);
}

User::User(char * _nick, char* _ident, char* _realname) {
    this->setNick(_nick);
    this->setIdent(_ident);
    this->setRealname(_realname);
}

User::~User() {
}

void User::setNick(char* _nick) {
	nick = new string(_nick);
}

void User::setIdent(char* _ident) {
	ident = new string(_ident);
}

void User::setRealname(char* _realname) {
	realname = new string(_realname);
}

void User::setHostmask(char* _hostmask) {
	hostmask = new string(_hostmask);
}

string* User::getNick() {
	return nick;
}

string* User::getIdent() {
	return ident;
}

string* User::getRealname() {
	return realname;
}

string* User::getHostmask() {
	return hostmask;
}

