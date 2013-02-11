#include "User.h"
#include <string>
#include "Parser.h"
#include "Bot.h"
#include "Adorabot.h"
#include <pthread.h>

using namespace std;
 
User::User(std::string _nick) {
    this->setNick(_nick);
    this->setIdent(_nick);
    this->setRealname(_nick);
    this->setUserid(0);
    server_ = 0;
}
 
User::User(std::string _nick, std::string _ident) {
    this->setNick(_nick);
    this->setIdent(_ident);
    this->setRealname(_ident);
    this->setUserid(0);
    server_ = 0;
}

User::User(std::string _nick, std::string _ident, std::string _realname) {
    this->setNick(_nick);
    this->setIdent(_ident);
    this->setRealname(_realname);
    this->setUserid(0);
    server_ = 0;
}

User::~User() {
    delete nick_;
    delete ident_;
    delete realname_;
    delete hostmask_;
    delete server_;
}

std::string User::parse(const std::string & _message) {
   return bot_->parse(_message);
}

void User::start() {
    connection_ = new Connection(this, server_);
    connection_->start();
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

void User::setUserid(int _id) {
    userid_ = _id;
}

void User::setServer(Server * _server) {
    server_ = _server;
}

void User::setBot(Bot* _bot) {
    bot_ = _bot;
}

void User::setThread(pthread_t & _thread) {
    thread_ = &_thread;
}

pthread_t* User::getThread() {
    return thread_;
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

int User::getUserid() {
    return userid_;
}

Server* User::getServer() {
    return server_;
}