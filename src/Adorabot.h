#ifndef _ADORABOT_H_
#define _ADORABOT_H_
#include "User.h"
#include <string.h>
#include "Parser.h"
#include "Bot.h"
#include "debug.h"

class Adorabot : public virtual Bot {
public:
    Adorabot();
    ~Adorabot();
 
    std::string parse(const std::string & _message);

 
private:
    std::string* server_;
    std::string* port_;
    int socket_; //the socket descriptor
    bool connected_;

    User* user_;

    std::string* in_;

    bool isConnected();

    void quit(std::string _msg);
};
 
#endif