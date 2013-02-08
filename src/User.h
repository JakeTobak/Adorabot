#ifndef _USER_H_
#define _USER_H_
#include <string>

class User {
public:
	User(std::string _nick);
	User(std::string _nick, std::string _ident);
    User(std::string _nick, std::string _ident, std::string _realname);
    virtual ~User();

    void setNick(std::string _nick);
    void setIdent(std::string _ident);
    void setRealname(std::string _realname);
    void setHostmask(std::string hostmask);

    std::string* getNick();
    std::string* getIdent();
    std::string* getRealname();
    std::string* getHostmask();
 
private:
    std::string* nick_;
    std::string* ident_;
    std::string* realname_;
    std::string* hostmask_;
};

#endif