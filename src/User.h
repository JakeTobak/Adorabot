#ifndef _USER_H_
#define _USER_H_
#include <string>
#include <string.h>
class User {
public:
	User(char* _nick);
	User(char* _nick, char* _ident);
    User(char* _nick, char* _ident, char* _realname);
    virtual ~User();

    void setNick(char* _nick);
    void setIdent(char* _ident);
    void setRealname(char* _realname);
    void setHostmask(char* hostmask);

    std::string* getNick();
    std::string* getIdent();
    std::string* getRealname();
    std::string* getHostmask();
 
private:
    std::string* nick;
    std::string* ident;
    std::string* realname;
    std::string* hostmask;
};

#endif