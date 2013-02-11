#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>

class Parser {
public:
	virtual ~Parser();
	virtual std::string parse(const std::string & _message) = 0;

private:

};

#endif