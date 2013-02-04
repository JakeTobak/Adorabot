#ifndef _ADORABOT_H_
#define _ADORABOT_H_
 
class Adorabot
{
public:
    Adorabot(char * _nick, char * _usr);
    Adorabot(User* _user);
    virtual ~Adorabot();
 
    bool setup;

    void start();
    bool charSearch(char *toSearch, char *searchFor);
 
private:
    char *port;
    int s; //the socket descriptor
 
    char *nick;
    char *usr;
 
    User* user;

    bool isConnected(char *buf);
    char * timeNow();
    bool sendData(char *msg);
    void sendPong(char *buf);
    void msgHandel(char *buf);
};
 
#endif