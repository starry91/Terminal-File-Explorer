#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <vector>
#include <string>
#include "page.h"

class CommandParser
{
    public:
    std::vector<std::string> getArgs(std::string, page_Sptr);    
};

#endif