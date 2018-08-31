#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <vector>
#include <string>
#include "page.h"

class CommandParser
{
    public:
    std::vector<std::string> getArgs(std::string);
    std::vector<std::string> translateArgs(std::vector<std::string>, page_Sptr page);
};

#endif