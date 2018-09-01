#ifndef FILE_H
#define FILE_H

#include <string>
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>
#include <memory>

class File
{
  struct stat fileStat;
  std::string dir_entry;
  public:
    std::string getUserName();
    std::string getGroupName();
    std::string getPermission();
    mode_t getFilePerms();
    size_t getSize();
    std::string getLastModified();
    char getFileType();
    std::string getFileName();
    File(std::string dir, std::string name);
    File(std::string path);
    File(std::string path, int);
};

typedef std::shared_ptr<File> file_Sptr;

#endif