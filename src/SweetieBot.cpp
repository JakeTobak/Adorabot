/*
 * SweetieBot.cpp
 *
 *  Created on: 15 Jul 2011
 *      Author: Tyler Allen
 */
 
#include "SweetieBot.h"
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

using namespace std;
 
#define MAXDATASIZE 100
 
SweetieBot::SweetieBot(char * _nick, char * _usr)
{
    nick = _nick;
    usr = _usr;
}
 
SweetieBot::~SweetieBot()
{
    close (s);
}
 
void SweetieBot::start()
{
    struct addrinfo hints, *servinfo;
 
    //Setup run with no errors
    setup = true;
 
    port = "6667";
 
    //Ensure that servinfo is clear
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
 
    //setup hints
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
 
    //Setup the structs if error print why
    int res;
    if ((res = getaddrinfo("irc.ecsig.com",port,&hints,&servinfo)) != 0)
    {
        setup = false;
        fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(res));
    }
 
    //setup the socket
    if ((s = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol)) == -1)
    {
        perror("client: socket");
    }
 
    //Connect
    if (connect(s,servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        close (s);
        perror("Client Connect");
    }
 
    //We dont need this anymore
    freeaddrinfo(servinfo);
 
    //Recv some data
    int numbytes;
    char buf[MAXDATASIZE];
 
    int count = 0;
    while (1)
    {
        //declars
        count++;
 
        switch (count) {
            case 3:
                    //after 3 recives send data to server (as per IRC protacol)
                    sendData(nick);
                    sendData(usr);
                break;
            case 4:
                    //Join a channel after we connect, this time we choose beaker
                sendData("JOIN #SweetieBot\r\n");
            default:
                break;
        }
 
        //Recv & print Data
        numbytes = recv(s,buf,MAXDATASIZE-1,0);
        buf[numbytes]='\0';
        cout << buf;
        //buf is the data that is recived
 
        //Pass buf to the message handeler
        msgHandel(buf);
 
        //If Ping Recived
        /*
         * must reply to ping overwise connection will be closed
         * see http://www.irchelp.org/irchelp/rfc/chapter4.html
         */
        if (charSearch(buf,"PING"))
        {
            sendPong(buf);
        }
 
        //break if connection closed
        if (numbytes==0)
        {
            cout << "----------------------CONNECTION CLOSED---------------------------"<< endl;
            cout << timeNow() << endl;
 
            break;
        }
    }
}
 
bool SweetieBot::charSearch(char *toSearch, char *searchFor)
{
    int len = strlen(toSearch);
    int forLen = strlen(searchFor); // The length of the searchfor field
 
    //Search through each char in toSearch
    for (int i = 0; i < len;i++)
    {
        //If the active char is equil to the first search item then search toSearch
        if (searchFor[0] == toSearch[i])
        {
            bool found = true;
            //search the char array for search field
            for (int x = 1; x < forLen; x++)
            {
                if (toSearch[i+x]!=searchFor[x])
                {
                    found = false;
                }
            }
 
            //if found return true;
            if (found == true)
                return true;
        }
    }
 
    return 0;
}
 
bool SweetieBot::isConnected(char *buf)
{//returns true if "/MOTD" is found in the input strin
    //If we find /MOTD then its ok join a channel
    if (charSearch(buf,"/MOTD") == true)
        return true;
    else
        return false;
}
 
char * SweetieBot::timeNow()
{//returns the current date and time
    time_t rawtime;
    struct tm * timeinfo;
 
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
 
    return asctime (timeinfo);
}
 
bool SweetieBot::sendData(char *msg)
{//Send some data
    //Send some data
    int len = strlen(msg);
    int bytes_sent = send(s,msg,len,0);
 
    if (bytes_sent == 0)
        return false;
    else
        return true;
}
 
void SweetieBot::sendPong(char *buf)
{
    //Get the reply address
    //loop through bug and find the location of PING
    //Search through each char in toSearch
 
    char * toSearch = "PING ";
 
    for (int i = 0; i < strlen(buf);i++)
        {
            //If the active char is equil to the first search item then search toSearch
            if (buf[i] == toSearch[0])
            {
                bool found = true;
                //search the char array for search field
                for (int x = 1; x < 4; x++)
                {
                    if (buf[i+x]!=toSearch[x])
                    {
                        found = false;
                    }
                }
 
                //if found return true;
                if (found == true)
                {
                    int count = 0;
                    //Count the chars
                    for (int x = (i+strlen(toSearch)); x < strlen(buf);x++)
                    {
                        count++;
                    }
 
                    //Create the new char array
                    char returnHost[count + 5];
                    returnHost[0]='P';
                    returnHost[1]='O';
                    returnHost[2]='N';
                    returnHost[3]='G';
                    returnHost[4]=' ';
 
                    count = 0;
                    //set the hostname data
                    for (int x = (i+strlen(toSearch)); x < strlen(buf);x++)
                    {
                        returnHost[count+5]=buf[x];
                        count++;
                    }
 
                    //send the pong
                    if (sendData(returnHost))
                    {
                        cout << timeNow() <<"  Ping Pong" << endl;
                    }
 
                    return;
                }
            }
        }
 
}
 
void SweetieBot::msgHandel(char * buf)
{
    /*
     * TODO: add you code to respod to commands here
     * the example below replys to the command hi scooby
     */
     string* msg = new string(buf);
    if (msg->find("?test") != -1)
    {
//        sendData("PRIVMSG #ecsig :no, u\r\n");
	CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://ecsig.com");
    /* example.com is redirected, so we tell libcurl to follow redirection */ 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
    }
 
}
