#ifndef _ADORABOT_H_
#define _ADORABOT_H_
#include "User.h"
class Adorabot {
public:
    Adorabot(User* _user);
    virtual ~Adorabot();
 
    void start();
    void stop();
 
private:
    char* port_;
    int socket_; //the socket descriptor
    bool connected_;
 
    User* user_;

    bool isConnected();
    void onConnect();
    bool sendData(char* _msg);
    bool sendData(std::string* _msg);

    void onReceive(char* _buf);
};
 
#endif