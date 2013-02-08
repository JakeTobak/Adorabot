/*
 * Adorabot.cpp
 */
 
#include "Adorabot.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
#include "ansi.h"

using namespace std;
 
#define MAXDATASIZE 1024
#define PING 1346981447

Adorabot::Adorabot(User* _user)
{
    user_ = _user;
}
 
Adorabot::~Adorabot()
{
    close (socket_);
}
 
void Adorabot::start(std::string _server, std::string _port)
{
    struct addrinfo hints, *servinfo;
    port_   = &_port;
    server_ = &_server;
 
    //Ensure that servinfo is clear
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
 
    //setup hints
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
 
    //Setup the structs if error print why
    int res;
    if ((res = getaddrinfo(server_->c_str(),port_->c_str(),&hints,&servinfo)) != 0)
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
                std::string nickmsg("NICK ");
                    nickmsg += *(user_->getNick());
                    nickmsg += "\r\n";
                sendData(nickmsg);

                std::string usermsg("USER ");
                    usermsg += *(user_->getIdent());
                    usermsg += " 8 * :";
                    usermsg += *(user_->getIdent());
                    usermsg += "\r\n";
                sendData(usermsg);
            }
        }
        //Recv & print Data
        char temp[1];
        for(int i=0; i<MAXDATASIZE-1; i++) {
            numbytes = recv(socket_,temp,1,0);
            buf[i] = temp[0];
            buf[i+1] = '\0';
            if(i > 0 && buf[i-1] == '\r' && buf[i] == '\n') {
                i = MAXDATASIZE;
            }
        }
        
        //ALWAYS RESPOND TO PINGS
        if (((int32_t*)buf)[0] == 1196312912) {
            buf[1] = 'O';
            in_ = new string(buf);
            sendData(in_);
        }

        in_ = new string(buf);
        
        onReceive(*in_);

        

        

 
        //break if connection closed
        if (numbytes==0) {
            cout << "----------------------CONNECTION CLOSED---------------------------"<< endl;
            break;
        }
    }
}

void Adorabot::stop() {
    quit("stop() called");
    exit(0);
}

void Adorabot::quit(std::string _msg) {
    sendData("QUIT " + _msg + "\r\n");
}
 
bool Adorabot::isConnected() {
    return connected_;
}

bool Adorabot::sendData(std::string* _msg) {

    cout << ANSIRED << ANSIBOLD << *_msg << ANSIRESET;
    // cout << "\x1b[31;1m" << *_msg << "\x1b[0m" << endl;

    //Send the message and see how many bytes were sent
    int bytes_sent = send(socket_,_msg->c_str(),_msg->length(),0);

    //IRC Protocol is supposed to silently ignore empty messages
    //so sending an extra \r\n won't hurt if the string already
    //had it, but if it didn't, then it will be sent here.
    send(socket_,"\r\n",2,0);

    //If no bytes were sent, then we had a problem...
    if (bytes_sent == 0)
        return false;
    else
        return true;
}

bool Adorabot::sendData(std::string _msg) {
    return sendData(&_msg);
}
 
void Adorabot::onConnect() {
    sendData("JOIN #Adorabot\r\n");
}

void Adorabot::onReceive(std::string _buf) {
    string* msg = new string(_buf);
    string* response = NULL;
    if(!connected_ && (int)(msg->find("/MOTD")) != -1) {
        connected_ = true;
        onConnect();
    }

    

    boost::regex rexp(".* PRIVMSG (#?\\w*) :([\\?\\w]*) ?(.*)\r?\n?");
    boost::smatch matches;
    boost::regex_search(*msg,matches,rexp);
    
    if(matches[0].matched) {
        cout << ANSIGREEN << ANSIBOLD;
    }
    cout << *msg << ANSIRESET;

    string all(matches[0].first,matches[0].second);
    string from(matches[1].first,matches[1].second);
    string command(matches[2].first,matches[2].second);
    string args(matches[3].first,matches[3].second);

    if(command.compare("?join") == 0) {
        string cmdraw("JOIN ");
        cmdraw += args.c_str();
        cmdraw += "\r\n";
        sendData(cmdraw);
    }
    else if(command.compare("?kill") == 0) {
        this->Adorabot::stop();
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