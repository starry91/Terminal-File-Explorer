#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <vector>
#include <string>
#include "page.h"

//Command handler to handle the command mode commands


class CommandHandler
{
  public:
    void copyFile(std::string file, std::string dir);
    void copyFiles(const std::vector<std::string> &files);
    void copyDir(std::string source_dir, std::string dest_dir);
    void delFiles(const std::vector<std::string> &files);
    void delFile(std::string file);
    void delDir(std::string source_dir);
    void rename(std::string old_name, std::string new_name);
    void createFile(std::string name, std::string dest_dir);
    void createDir(std::string name, std::string dest_dir);
    void search(std::string name, std::string dir, std::vector<std::string>& output);
    void snapshot(std::string dir, std::string file);
    page_Sptr goToDir(std::string dir);
};

#endif