#include "CommandParser.h"
#include "path.h"
#include <syslog.h>
std::vector<std::string> CommandParser::getArgs(std::string command)
{
    std::vector<std::string> res;
    int i = 0;
    bool found = false;
    auto path_obj = Path::getInstance();
    for (int start = 0; start <= command.length(); start++)
    {
        if ((command[start] == ' ' || start == command.length()) && found == false)
        {
            res.push_back(command.substr(i, start - i));
            i = start + 1;
        }
    }
    return res;
};

std::vector<std::string> CommandParser::translateArgs(std::vector<std::string> args, page_Sptr page)
{
    std::vector<std::string> processed_output;
    auto path_obj = Path::getInstance();
    for (int start = 0; start < args.size(); start++)
    {
        if (start == 0)
        {
            processed_output.push_back(args[start]);
            //syslog(0, "Arg1: [%s]", command.substr(i, start).c_str());
        }
        else if (args[start][0] == '.')
        {
            processed_output.push_back(page->cwd + "/" + args[start].substr(1, args[start].length() - 1));
            //syslog(0, "Arg2: [%s]", (page->cwd + "/" + command.substr(i + 1, start - i)).c_str());
        }
        else if (args[start][0] == '/')
        {
            processed_output.push_back(path_obj.getHomePath() + args[start]);
            //syslog(0, "Arg3: [%s]", (path_obj.getHomePath() + command.substr(i, start - i + 1)).c_str());
        }
        else if (args[start][0] == '~')
        {
            processed_output.push_back(path_obj.getHomePath() + "/" + args[start].substr(1, args[start].length() - 1));
            //syslog(0, "Arg5: [%s]", (path_obj.getHomePath() + command.substr(i + 1, start - i)).c_str());
        }
        else
        {
            processed_output.push_back(page->cwd + "/" + args[start]);
            //syslog(0, "Arg6: [%s]", (page->cwd + "/" + command.substr(i, start - i)).c_str());
        }
    }
    return processed_output;
}
