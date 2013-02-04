#ifndef _USER_H_
#define _USER_H_

class User
{
public:
	User(char* _nick);
	User(char* _nick, char* _ident);
    User(char* _nick, char* _ident, char* _realname);
    virtual ~User();

    void setNick(char* _nick);
    void setIdent(char* _ident);
    void setRealname(char* _realname);
    void setHostmask(char* hostmask);

    string getNick();
    string getIdent();
    string getRealname();
    string getHostmask();
 
private:
    string* nick;
    string* ident;
    string* realname;
    string* hostmask;
};

#endif