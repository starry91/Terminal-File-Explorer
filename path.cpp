//Name: Praveen Balireddy
//Roll: 2018201052

#include "path.h"

Path::Path()
{
};

void Path::setHomePath(std::string home_path)       //setting application HOME/ROOT path
{
    this->HOME_APPLICATION = home_path;
};

std::string Path::getHomePath()             //getting application HOME path
{
    return this->HOME_APPLICATION;
};

Path &Path::getInstance()           //getting singleton instance of the path object
{
    static Path path;
    return path;
}

std::string Path::getParentDir(std::string dir)         //getting parent path of the given dir
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

std::string Path::getCurrDir(std::string dir)           //getting current dir
{
    return dir;
};

std::string Path::getAppAbsPath(std::string dir)        //getting application ablsolute path
{
    if (dir.length() > this->HOME_APPLICATION.length() && dir.substr(0, this->HOME_APPLICATION.length()) == this->HOME_APPLICATION)
        return dir.substr(this->HOME_APPLICATION.length(), dir.length() - this->HOME_APPLICATION.length());
    if (dir.length() == this->HOME_APPLICATION.length() && dir.substr(0, this->HOME_APPLICATION.length()) == this->HOME_APPLICATION)
        return "/";
    else if (dir.length() >= this->HOME_APPLICATION.length())
        return dir;
    else
        return std::string("");
}

std::string Path::getSystemAbsPath(std::string dir)         //getting system absolute path
{
    if (dir.length() >= this->HOME_APPLICATION.length() && dir.substr(0, this->HOME_APPLICATION.length()) == this->HOME_APPLICATION)
        return dir;
    else if (dir[0] != '/')
        return this->HOME_APPLICATION + "/" + dir;
    else
        return this->HOME_APPLICATION + dir;
}