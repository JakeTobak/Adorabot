#ifndef _SWEETIEBOT_H_
#define _SWEETIEBOT_H_
 
class SweetieBot
{
public:
    SweetieBot(char * _nick, char * _usr);
    virtual ~SweetieBot();
 
    bool setup;
 
    void start();
    bool charSearch(char *toSearch, char *searchFor);
 
private:
    char *port;
    int s; //the socket descriptor
 
    char *nick;
    char *usr;
 
    bool isConnected(char *buf);
    char * timeNow();
    bool sendData(char *msg);
    void sendPong(char *buf);
    void msgHandel(char *buf);
};
 
#endif