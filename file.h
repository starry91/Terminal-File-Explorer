//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef FILE_H
#define FILE_H

#include <string>
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>
#include <memory>

//File Class to store information related to a single file


class File
{
  struct stat fileStat;
  struct stat effFileStat;
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
  File(std::string dir, std::string name);        //uses lstat
  File(std::string path);
  File(std::string path, int);                    
  File(std::string file, char stat_type);         //uses stat //for search mode
  char getEffFileType();
};

typedef std::shared_ptr<File> file_Sptr;

#endif