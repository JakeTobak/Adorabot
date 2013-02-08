#ifndef _ADORABOT_H_
#define _ADORABOT_H_
#include "User.h"
#include <string.h>
class Adorabot {
public:
    Adorabot(User* _user);
    virtual ~Adorabot();
 
    void start(std::string _server, std::string _port);
    void stop();
 
private:
    std::string* server_;
    std::string* port_;
    int socket_; //the socket descriptor
    bool connected_;

    User* user_;

    std::string* in_;

    bool isConnected();
    void onConnect();
    bool sendData(std::string _msg);
    bool sendData(std::string* _msg);

    void onReceive(std::string _buf);

    void quit(std::string _msg);
};
 
#endif