#ifndef _MESSAGEFACTORY_H_
#define _MESSAGEFACTORY_H_
#include "debug.h"
#include <string>

class MessageFactory {
public:
	static std::string* makeMessage(std::string const& _message);

private:

};

#endif