#include "CommandParser.h"
#include "path.h"
#include <syslog.h>
#include <iostream>
#include <cstring>
enum PARSE_MODE
{
    SPACE = 0,
    SINGLE_QUOTE = 1,
    DOUBLE_QUOTE = 2,
    ESCAPE = 3,
};

std::vector<std::string> CommandParser::getArgs(std::string command)
{
    std::vector<std::string> res;
    int i = 0;
    bool found = false;
    auto path_obj = Path::getInstance();
    int mode = PARSE_MODE::SPACE;
    int prev_mode, start_index, end_index;
    //start_index = 0;
    char buff[1024];
    int buff_index = 0;
    std::memset(buff, 0, 1024);
    for (int start = 0; start <= command.length(); start++)
    {
        if (start == command.length() && buff_index > 0)
        {
            buff[buff_index] = 0;
            res.push_back(std::string(buff));
        }
        if (mode == PARSE_MODE::SPACE)
        {
            syslog(0, "Entering NORMAL MODE: [%s]",buff);
            if (command[start] == '\\')
            {
                syslog(0, "switch escape");
                mode = PARSE_MODE::ESCAPE;
                prev_mode = PARSE_MODE::SPACE;
            }
            else if (command[start] == ' ')
            {
                syslog(0, "continue normal: [%s]",buff);
                buff[buff_index] = 0;
                res.push_back(std::string(buff));
                start_index = start + 1;
                buff_index = 0;
                std::memset(buff, 0, 1024);
            }
            else if (command[start] == '\'')
            {
                syslog(0, "switch single quote: [%s]",buff);
                mode = PARSE_MODE::SINGLE_QUOTE;
                prev_mode = PARSE_MODE::SPACE;
            }
            else if (command[start] == '"')
            {
                syslog(0, "switch double quote: [%s]",buff);
                mode = PARSE_MODE::DOUBLE_QUOTE;
                prev_mode = PARSE_MODE::SPACE;
            }
            else
            {
                syslog(0, "add buffer: [%s]",buff);
                buff[buff_index] = command[start];
                buff_index++;
            }
        }
        else if (mode == PARSE_MODE::ESCAPE)
        {
            buff[buff_index] = command[start];
            buff_index++;
            mode = prev_mode;
            syslog(0, "exiting escape: [%s]",buff);
        }
        else if (mode == PARSE_MODE::SINGLE_QUOTE)
        {
            if (command[start] == '\'')
            {
                buff[buff_index] = 0;
                res.push_back(std::string(buff));
                start_index = start + 1;
                buff_index = 0;
                std::memset(buff, 0, 1024);
                mode = prev_mode;
                syslog(0, "exiting single quote: [%s]",buff);
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
                start_index = start + 1;
                buff_index = 0;
                std::memset(buff, 0, 1024);
                mode = prev_mode;
                syslog(0, "exiting double quote: [%s]",buff);
            }
            else
            {
                buff[buff_index] = command[start];
                buff_index++;
            }
        }
        // if ((command[start] == ' ' || start == command.length()) && found == false)
        // {
        //     res.push_back(command.substr(i, start - i));
        //     i = start + 1;
        // }
    }
    return res;
};

std::vector<std::string> CommandParser::translateArgs(std::vector<std::string> args, page_Sptr page)
{
    std::vector<std::string> processed_output;
    auto path_obj = Path::getInstance();
    for (int start = 0; start < args.size(); start++)
    {
        syslog(0, "Loop: [%s]", args[start].c_str());
        if (start == 0)
        {
            processed_output.push_back(args[start]);
            //syslog(0, "Arg1: [%s]", command.substr(i, start).c_str());
        }
        else if (args[start][0] == '.')
        {
            if (args[start].length() > 1)
                processed_output.push_back(page->cwd + args[start].substr(1, args[start].length() - 1));
            else
                processed_output.push_back(page->cwd);
            //syslog(0, "Arg2: [%s]", (page->cwd + "/" + command.substr(i + 1, start - i)).c_str());
        }
        else if (args[start][0] == '/')
        {
            if (args[start].length() >= path_obj.getHomePath().length() && args[start].substr(0, path_obj.getHomePath().length()) == path_obj.getHomePath())
                processed_output.push_back(args[start]);
            if (args[start].length() > 1)
                processed_output.push_back(path_obj.getHomePath() + args[start]);
            else
                processed_output.push_back(path_obj.getHomePath());
            //syslog(0, "Arg3: [%s]", (path_obj.getHomePath() + command.substr(i, start - i + 1)).c_str());
        }
        else if (args[start][0] == '~')
        {
            if (args[start].length() > 1)
                processed_output.push_back(path_obj.getHomePath() + "/" + args[start].substr(1, args[start].length() - 1));
            else
                processed_output.push_back(path_obj.getHomePath());
            //syslog(0, "Arg5: [%s]", (path_obj.getHomePath() + command.substr(i + 1, start - i)).c_str());
        }
        else
        {
            syslog(0, "Abcsda");
            syslog(0, "Arg6: [%s]", (page->cwd.c_str()));
            processed_output.push_back(page->cwd + "/" + args[start]);
            //syslog(0, "Arg6: [%s]", (page->cwd + "/" + command.substr(i, start - i)).c_str());
        }
    }
    return processed_output;
}