#include "MessageFactory.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <boost/xpressive/xpressive.hpp>

namespace xpres = boost::xpressive;

std::string* MessageFactory::makeMessage(std::string const& _message) {
	//xpres::sregex rexp = xpres::sregex::compile("^(:?(?P<prefix>([a-zA-Z0-9\\.\\-]+)|([a-zA-Z][a-zA-Z0-9\\-\\[\\]\\\\`^\\{\\}]*(!\\S+)?(@[a-zA-Z0-9\\.-]+)?)))\\s(?P<command>([0-9]{3}|[a-zA-Z]+)\\s?(?P<arg1>[^:\\s\\n\\r])?\\s?:?(?P<params>\\S*))\\r\\n$");
    	
	xpres::sregex rexp = xpres::sregex::compile("^(:?(?P<prefix>([a-zA-Z0-9\\.\\-]+)|([a-zA-Z][a-zA-Z0-9\\-\\[\\]\\\\`^\\{\\}]*(!\\S+)?(@[a-zA-Z0-9\\.-]+)?)))\\s.*\\r\\n$");

    	//xpres::sregex rexp = xpres::sregex::compile("^:?(?P<prefix>[a-zA-Z0-9\\.-]+).*\r\n$");

    	// boost::regex rexp("^.*(?<test>[a-zA-Z0-9\\.-]+) .*\r\n$");

    xpres::smatch matches;
    xpres::regex_search(_message,matches,rexp);
    
    // if(matches[0].matched) {
    //     cout << ANSIGREEN << ANSIBOLD;
    // }

    // string all(matches[0].first,matches[0].second);
    // string from(matches[1].first,matches[1].second);
    // string command(matches[2].first,matches[2].second);
    // string args(matches[3].first,matches[3].second);
    std::string * prefix;
    if(matches["prefix"].matched) {
    	prefix = new std::string(matches["prefix"].first,matches["prefix"].second);
    }
    else {
    	prefix = new std::string(" ");
    }

    #ifdef _ADORABOT_DEBUG_
	   std::cout << "MessageFactory:\n\r\tPrefix: " << *prefix << std::endl;;
    #endif

    return prefix;

    // if(command.compare("?join") == 0) {
    //     string cmdraw("JOIN ");
    //     cmdraw += args.c_str();
    //     response += cmdraw;
    // }
}