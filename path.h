
#ifndef PATH_H
#define PATH_H

#include <string>

class Path
{
  protected:
    std::string HOME_APPLICATION;
    Path(){};

  public:
    static Path &getInstance();
    void setHomePath(std::string);
    std::string getHomePath();
    std::string getSystemAbsPath(std::string dir);
    std::string getAppAbsPath(std::string dir);
    std::string getParentDir(std::string dir);
    std::string getCurrDir(std::string dir);
}; // namespace Path

#endif