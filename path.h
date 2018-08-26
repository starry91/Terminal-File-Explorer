
#ifndef PATH_H
#define PATH_H

#include <string>

namespace Path
{
std::string getParentDir(std::string dir);
std::string getCurrDir(std::string dir);
}; // namespace Path

#endif