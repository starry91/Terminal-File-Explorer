#include "CommandParser.h"

std::vector<std::string> CommandParser::getArgs(std::string command)
{
    std::vector<std::string> res;
    int i = 0;
    bool found = false;
    for (int start = 0; start < command.length(); size++)
    {
        if (command[start] == " " && found == false)
        {
            res.push_back(command.substr(i, start));
            i = start + 1;
        }
        //else if(command[start] == "\"")
    }
    return res;
};
