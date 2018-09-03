//Name: Praveen Balireddy
//Roll: 2018201052

#include "CommandParser.h"
#include "path.h"
#include <iostream>
#include <cstring>
#include <syslog.h>

enum PARSE_MODE //Parse mode(State machin states) for parsing the user command
{
    SPACE = 0,
    SINGLE_QUOTE = 1,
    DOUBLE_QUOTE = 2,
    ESCAPE = 3,
};

std::vector<std::string> CommandParser::getArgs(std::string command) //parsing the user command text
{
    std::vector<std::string> res;
    int i = 0;
    bool found = false;
    auto path_obj = Path::getInstance();
    int mode = PARSE_MODE::SPACE;
    int prev_mode;
    //start_index = 0;
    int space_count = 0;
    char buff[1024];
    int buff_index = 0;
    std::memset(buff, 0, 1024);
    for (int start = 0; start <= command.length(); start++)
    {
        if (start == command.length() && buff_index > 0)
        {
            buff[buff_index] = 0;
            res.push_back(std::string(buff));
            space_count = 0;
        }
        if (mode == PARSE_MODE::SPACE)
        {
            if (command[start] == '\\')
            {
                mode = PARSE_MODE::ESCAPE;
                prev_mode = PARSE_MODE::SPACE;
            }
            else if (command[start] == ' ')
            {
                if (space_count > 0 || start == 0)
                {
                    space_count++;
                }
                else
                {
                    buff[buff_index] = 0;
                    res.push_back(std::string(buff));
                    buff_index = 0;
                    std::memset(buff, 0, 1024);
                    space_count++;
                }
            }
            else if (command[start] == '\'')
            {
                mode = PARSE_MODE::SINGLE_QUOTE;
                prev_mode = PARSE_MODE::SPACE;
                space_count = 0;
            }
            else if (command[start] == '"')
            {
                mode = PARSE_MODE::DOUBLE_QUOTE;
                prev_mode = PARSE_MODE::SPACE;
                space_count = 0;
            }
            else
            {
                buff[buff_index] = command[start];
                buff_index++;
                space_count = 0;
            }
        }
        else if (mode == PARSE_MODE::ESCAPE)
        {
            buff[buff_index] = command[start];
            buff_index++;
            mode = prev_mode;
        }
        else if (mode == PARSE_MODE::SINGLE_QUOTE)
        {
            if (command[start] == '\'')
            {
                buff[buff_index] = 0;
                res.push_back(std::string(buff));
                buff_index = 0;
                std::memset(buff, 0, 1024);
                mode = prev_mode;
            }
            else
            {
                buff[buff_index] = command[start];
                buff_index++;
            }
        }
        else if (mode == PARSE_MODE::DOUBLE_QUOTE)
        {
            if (command[start] == '"')
            {
                buff[buff_index] = 0;
                res.push_back(std::string(buff));
                buff_index = 0;
                std::memset(buff, 0, 1024);
                mode = prev_mode;
            }
            else
            {
                buff[buff_index] = command[start];
                buff_index++;
            }
        }
    }
    return res;
};

std::vector<std::string> CommandParser::translateArgs(std::vector<std::string> args, page_Sptr page) //translating the args to system absolute paths
{
    std::vector<std::string> processed_output;
    auto path_obj = Path::getInstance();
    for (int start = 0; start < args.size(); start++)
    {
        if (start == 0)
        {
            processed_output.push_back(args[start]);
        }
        else if (args[start][0] == '.' && ((args[start].length() > 1 && args[start][1] == '/') || args[start].length() == 1))
        {
            if (args[start].length() > 1)
                processed_output.push_back(page->cwd + args[start].substr(1, args[start].length() - 1));
            else
                processed_output.push_back(page->cwd);
        }
        else if (args[start][0] == '/')
        {
            if (args[start].length() >= path_obj.getHomePath().length() && args[start].substr(0, path_obj.getHomePath().length()) == path_obj.getHomePath())
                processed_output.push_back(args[start]);
            if (args[start].length() > 1)
                processed_output.push_back(path_obj.getHomePath() + args[start]);
            else
                processed_output.push_back(path_obj.getHomePath());
        }
        else if (args[start][0] == '~')
        {
            if (args[start].length() > 1)
                processed_output.push_back(path_obj.getHomePath() + "/" + args[start].substr(1, args[start].length() - 1));
            else
                processed_output.push_back(path_obj.getHomePath());
        }
        else
        {
            processed_output.push_back(page->cwd + "/" + args[start]);
        }
    }
    return processed_output;
}