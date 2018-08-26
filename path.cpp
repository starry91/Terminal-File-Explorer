#include "path.h"

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
    return dir.substr(0, start+1);
};

std::string Path::getCurrDir(std::string dir)
{
    return dir;
};