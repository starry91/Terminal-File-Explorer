#ifndef FILEATTR_H
#define FILEATTR_H

#include <string>
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>

class fileAttr
{
  public:
    std::string permission;
    std::string usr_name;
    std::string grp_name;
    long long size;
    std::string last_modified;
    std::string name;
    char file_type;
    fileAttr(std::string perm, std::string usr_name,
             std::string grp_name, long long size,
             std::string last_modified, std::string name);
};

#endif