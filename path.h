
#ifndef PATH_H
#define PATH_H

#include <string>

//Path class to handle path translation between system and application root and other path queries


class Path
{
private:
  std::string HOME_APPLICATION;
  Path();

public:
  static Path &getInstance();
  void setHomePath(std::string);
  std::string getHomePath();
  std::string getSystemAbsPath(std::string dir);
  std::string getAppAbsPath(std::string dir);
  std::string getParentDir(std::string dir);
  std::string getCurrDir(std::string dir);
};

#endif