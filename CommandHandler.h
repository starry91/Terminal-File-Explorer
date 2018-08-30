#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <vector>
#include <string>

class CommandHandler
{
    public:
    void copyFile(std::string file,std::string dir);
    void copyFiles(const std::vector<std::string>& files);     
};

#endif