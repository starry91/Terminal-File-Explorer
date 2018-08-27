
#ifndef PATH_H
#define PATH_H

#include <string>

namespace Path
{
extern std::string HOME_APPLICATION;
std::string getSystemAbsPath(std::string dir);
std::string getAppAbsPath(std::string dir);
std::string getParentDir(std::string dir);
std::string getCurrDir(std::string dir);
}; // namespace Path

#endif