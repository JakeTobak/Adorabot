/*
 * Adorabot.cpp
 */
 
#include "Adorabot.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <mysql/mysql.h>
#include <curl/curl.h>
#include <boost/regex.hpp>

#include "Message.h"
#include "User.h"

using namespace std;
 
#define MAXDATASIZE 1024

Adorabot::Adorabot(User* _user)
{
    user_ = _user;
}
 
Adorabot::~Adorabot()
{
    close (socket_);
}
 
void Adorabot::start()
{
    struct addrinfo hints, *servinfo;

    port_ = (char*)"6667";
 
    //Ensure that servinfo is clear
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
 
    //setup hints
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
 
    //Setup the structs if error print why
    int res;
    if ((res = getaddrinfo("irc.ecsig.com",port_,&hints,&servinfo)) != 0)
    {
        fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(res));
    }
 
    //setup the socket
    if ((socket_ = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol)) == -1)
    {
        perror("client: socket");
    }
 
    //Connect
    if (connect(socket_,servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        close (socket_);
        perror("Client Connect");
    }
 
    //We dont need this anymore
    freeaddrinfo(servinfo);
 
    //Recv some data
    int numbytes;
    char buf[MAXDATASIZE];
    int count = 0;
    while (1) {
        memset(buf,'\0',MAXDATASIZE);
        
        if(count<3) {
            count++;
     
            if(count == 3) {
                char nickmsg[100];
                strcpy(nickmsg,"NICK ");
                strcat(nickmsg,user_->getNick()->c_str());
                strcat(nickmsg,"\r\n");
                sendData(nickmsg);

                char usermsg[100];
                strcpy(usermsg,"USER ");
                strcat(usermsg,user_->getIdent()->c_str());
                strcat(usermsg," 8 * :");
                strcat(usermsg,user_->getIdent()->c_str());
                strcat(usermsg,"\r\n");
                sendData(usermsg);
            }
        }
        //Recv & print Data
        char temp[1];
        for(int i=0; i<MAXDATASIZE-1; i++) {
            numbytes = recv(socket_,temp,1,0);
            buf[i] = temp[0];
            buf[i+1] = '\0';
            if(i > 1 && buf[i-1] == '\r' && buf[i] == '\n') {
                i = MAXDATASIZE;
            }
        }
        
        onReceive(buf);

        //ALWAYS RESPOND TO PINGS
        if (buf[0] == 'P' && buf[1] == 'I' && buf[2] == 'N' && buf[3] == 'G') {
            buf[1] = 'O';
            sendData(buf);
        }

        

 
        //break if connection closed
        if (numbytes==0) {
            cout << "----------------------CONNECTION CLOSED---------------------------"<< endl;
            break;
        }
    }
}

void Adorabot::stop() {
    sendData((char*)"QUIT stop() called");
    close(socket_);
}
 
bool Adorabot::isConnected() {
    return connected_;
}

bool Adorabot::sendData(char* _msg) {
    cout << _msg << endl;
    int len = strlen(_msg);
    int bytes_sent = send(socket_,_msg,len,0);
    if (bytes_sent == 0)
        return false;
    else
        return true;
}

bool Adorabot::sendData(std::string* _msg) {
    int len = strlen(_msg->c_str());
    char mymessage[len];
    strcpy(mymessage,_msg->c_str());
    return sendData(mymessage);
}
 
void Adorabot::onConnect() {
    sendData((char*)"JOIN #Adorabot\r\n");
}

void Adorabot::onReceive(char* _buf) {
    string* msg = new string(_buf);
    string* response = NULL;

    if(!connected_ && (int)(msg->find("/MOTD")) != -1) {
        connected_ = true;
        onConnect();
    }

    cout << *msg;

    boost::regex rexp(".* PRIVMSG (#?\\w*) :([\\?\\w]*) ?(.*)\r?\n?");
    boost::smatch matches;
    boost::regex_search(*msg,matches,rexp);
    
    string all(matches[0].first,matches[0].second);
    string from(matches[1].first,matches[1].second);
    string command(matches[2].first,matches[2].second);
    string args(matches[3].first,matches[3].second);

    if(command.compare("?join") == 0) {
        string cmdraw("JOIN ");
        cmdraw += args.c_str();
        cmdraw += "\r\n";
        sendData(&cmdraw);
    }
    else if(command.compare("?part") == 0) {
        char cmdraw[100];
        strcpy(cmdraw,"PART ");
        strcat(cmdraw,args.c_str());
        strcat(cmdraw,"\r\n");
                    
        response = new string(cmdraw);
        sendData(response);
    }
    else if((int)(all.find(":Who's adorable")) != -1) {
        response = new string("PRIVMSG ");
        *response += from.c_str();
        *response += " :\\(^o^)/ I am! \r\n";
        sendData(response);
    }

    delete msg;
    delete response;
}











    // if (msg->find("?test") != -1)
    // {
//        sendData("PRIVMSG #ecsig :no, u\r\n");
    // CURL *curl;
 //  CURLcode res;
 
 //  curl = curl_easy_init();
 //  if(curl) {
 //    curl_easy_setopt(curl, CURLOPT_URL, "http://ecsig.com");
 //    /* example.com is redirected, so we tell libcurl to follow redirection */ 
 //    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
 //     Perform the request, res will get the return code  
 //    res = curl_easy_perform(curl);
 //    /* Check for errors */ 
 //    if(res != CURLE_OK)
 //      fprintf(stderr, "curl_easy_perform() failed: %s\n",
 //              curl_easy_strerror(res));
 
 //    /* always cleanup */ 
 //    curl_easy_cleanup(curl);
 //  }
    //}