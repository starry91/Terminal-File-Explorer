#ifndef FILE_H
#define FILE_H

#include <string>
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>
#include <memory>

class File
{
  public:
    std::string permission;
    std::string usr_name;
    std::string grp_name;
    long long size;
    std::string last_modified;
    std::string name;
    char file_type;
    
    File(std::string);
};

typedef std::shared_ptr<File> file_Sptr;

#endif