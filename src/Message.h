#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include <string.h>

class Message {
public:

private:
	std::string* raw_;
	std::string* type_;
	std::string* from_;
	std::string* content_;
};

#endif