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

#include "User.h"
#include "ansi.h"

using namespace std;
 


Adorabot::Adorabot() {
}
 
Adorabot::~Adorabot() {

}

void Adorabot::quit(std::string _msg) {

}
 
bool Adorabot::isConnected() {
    return connected_;
}

std::string Adorabot::parse(std::string const& _message) {
   std::string response("");

    if(!connected_ && (int)(_message.find("MOTD")) != -1) {
        connected_ = true;
        response += "JOIN #Adorabot\r\n";
    }

    

    boost::regex rexp(".* PRIVMSG (#?\\w*) :([\\?\\w]*) ?(.*)\r?\n?");
    boost::smatch matches;
    boost::regex_search(_message,matches,rexp);
    
    if(matches[0].matched) {
        cout << ANSIGREEN << ANSIBOLD;
    }

    string all(matches[0].first,matches[0].second);
    string from(matches[1].first,matches[1].second);
    string command(matches[2].first,matches[2].second);
    string args(matches[3].first,matches[3].second);

    if(command.compare("?join") == 0) {
        string cmdraw("JOIN ");
        cmdraw += args.c_str();
        response += cmdraw;
    }
    else if(command.compare("?kill") == 0) {
        //this->Adorabot::stop();
    }
    else if(command.compare("?part") == 0) {
        char cmdraw[100];
        strcpy(cmdraw,"PART ");
        strcat(cmdraw,args.c_str());
        response += std::string(cmdraw);
    }
    else if((int)(all.find(":Who's adorable")) != -1) {
        response += "PRIVMSG ";
        response += from.c_str();
        response += " :\\(^o^)/ I am!";
    }
    return response;
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