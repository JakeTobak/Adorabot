#include "Connection.h"

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
#include <string>

#include "User.h"
#include "Server.h"
#include "ansi.h"
#include "MessageFactory.h"

#define MAXDATASIZE 1024
#define PING 1346981447

using namespace std;

Connection::Connection(User* _user, Server* _server) {
	user_	 = _user;
	server_ = _server;
}

Connection::~Connection() {
	delete user_;
	delete server_;
	if(connected_)
		close (socket_);
}

void Connection::start() {
	struct addrinfo hints, *servinfo;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    int res;
    if ((res = getaddrinfo(server_->getAddress()->c_str(),server_->getPort()->c_str(),&hints,&servinfo)) != 0)
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

        freeaddrinfo(servinfo);

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
        
        in_ = new string(buf);
        
        cout << *(MessageFactory::makeMessage(*in_)) << endl;

        //ALWAYS RESPOND TO PINGS
        if (((int32_t*)buf)[0] == 1196312912) {
            cout << ANSIYELLOW << ANSIBOLD << buf << ANSIRESET;
            buf[1] = 'O';
            in_ = new string(buf);
            sendData(in_);
        }
        else {
        	cout << ANSIYELLOW << buf << ANSIRESET;
            sendData(parse(*in_));
        }

        
        
        

        

        

 
        //break if connection closed
        if (numbytes==0) {
            cout << "----------------------CONNECTION CLOSED---------------------------"<< endl;
            break;
        }
    }

}
void Connection::stop() {

}

std::string Connection::parse(const std::string & _message) {
	return user_->parse(_message);
}

bool Connection::sendData(std::string* _msg) {
	*_msg += "\r\n";
    cout << ANSIGREEN << *_msg << ANSIRESET;
    // cout << "\x1b[31;1m" << *_msg << "\x1b[0m" << endl;

    //Send the message and see how many bytes were sent
    int bytes_sent = send(socket_,_msg->c_str(),_msg->length(),0);

    //If no bytes were sent, then we had a problem...
    if (bytes_sent == 0)
        return false;
    else
        return true;
}

bool Connection::sendData(std::string _msg) {
    return sendData(&_msg);
}