#include "CommandParser.h"
#include "path.h"
#include <syslog.h>
std::vector<std::string> CommandParser::getArgs(std::string command, page_Sptr page)
{
    std::vector<std::string> res;
    int i = 0;
    bool found = false;
    for (int start = 0; start <= command.length(); start++)
    {
        if ((command[start] == ' ' || start == command.length()) && found == false)
        {
            if (res.size() == 0)
            {
                res.push_back(command.substr(i, start));
                syslog(0, "Arg1: [%s]",command.substr(i, start).c_str());
            }
            else if (command[i] == '.')
            {
                res.push_back(page->cwd + "/" + command.substr(i + 1, start - i));
                syslog(0, "Arg2: [%s]",(page->cwd + "/" + command.substr(i + 1, start - i)).c_str());
            }
            else if (command[i] == '/')
            {
                res.push_back(Path::HOME_APPLICATION + command.substr(i, start - i + 1));
                syslog(0, "Arg3: [%s]",(Path::HOME_APPLICATION + command.substr(i, start - i + 1)).c_str());
            }
            else if (command[i] == '~')
            {
                res.push_back(Path::HOME_APPLICATION + command.substr(i + 1, start - i));
                syslog(0, "Arg5: [%s]",(Path::HOME_APPLICATION + command.substr(i + 1, start - i)).c_str());
            }
            else
            {
                res.push_back(page->cwd + "/" + command.substr(i, start - i));
                syslog(0, "Arg6: [%s]", (page->cwd + "/" + command.substr(i, start - i)).c_str());
            }
            i = start + 1;
        }
    }
    return res;
};
