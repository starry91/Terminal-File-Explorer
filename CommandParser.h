//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <vector>
#include <string>
#include "page.h"

//Command parser to parse user commands


class CommandParser
{
    public:
    std::vector<std::string> getArgs(std::string);                  //split the command text in arguments(used state machine design to parse)
    std::vector<std::string> translateArgs(std::vector<std::string>, page_Sptr page);       //translate args to system absolute paths
};

#endif