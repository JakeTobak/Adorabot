#ifndef _BOT_H_
#define _BOT_H_
#include "Parser.h"
#include "debug.h"
class Bot : public virtual Parser {
public:
	Bot();
	virtual ~Bot();
	// std::string parse(const std::string & _message);

private:

};

#endif