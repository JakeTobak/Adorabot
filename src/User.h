#ifndef _USER_H_
#define _USER_H_
#include <string>
#include "Parser.h"
#include "Server.h"
#include "Connection.h"
#include <pthread.h>
#include "debug.h"
class Connection;
class Bot; 

class User : public virtual Parser {
public:
	User(std::string _nick);
	User(std::string _nick, std::string _ident);
    User(std::string _nick, std::string _ident, std::string _realname);
    ~User();

    std::string parse(const std::string & _message);
    void start();

    void setNick(std::string _nick);
    void setIdent(std::string _ident);
    void setRealname(std::string _realname);
    void setHostmask(std::string _hostmask);
    void setUserid(int _id);
    void setServer(Server * _server);
    void setBot(Bot* _bot);
    void setThread(pthread_t & _thread);

    std::string* getNick();
    std::string* getIdent();
    std::string* getRealname();
    std::string* getHostmask();
    Server * getServer();
    int getUserid();
    pthread_t* getThread();

private:
    std::string * nick_;
    std::string * ident_;
    std::string * realname_;
    std::string * hostmask_;
    Server * server_;
    Connection * connection_;
    int userid_;
    Bot * bot_;
    pthread_t * thread_;
};

#endif