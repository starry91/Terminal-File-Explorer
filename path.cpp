#include "path.h"
#include <syslog.h>

std::string Path::getParentDir(std::string dir)
{
    int count = 0, start = dir.length() - 1;

    while (count < 1)
    {
        if (dir[start] == '/')
        {
            count++;
        }
        start--;
    }
    return dir.substr(0, start + 1);
};

std::string Path::getCurrDir(std::string dir)
{
    return dir;
};

std::string Path::getAppAbsPath(std::string dir)
{
    syslog(0, "Path: %s Home: %s", dir.c_str(), Path::HOME_APPLICATION.c_str());
    if (dir.length() >= Path::HOME_APPLICATION.length())
        return dir.substr(Path::HOME_APPLICATION.length(), dir.length() - 1);
    else
        return std::string("");
}

std::string Path::getSystemAbsPath(std::string dir)
{
    return Path::HOME_APPLICATION + dir;
}