#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <vector>
#include <string>

class CommandHandler
{
    public:
    void copyFile(std::string file,std::string dir);
    void copyFiles(const std::vector<std::string>& files);
    void copyDir(std::string source_dir, std::string dest_dir);     
};

#endif