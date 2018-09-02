#ifndef Error_H
#define Error_H

#include <string>
#include <iostream>

class Error
{
    std::string msg;
    public:
    Error(std::string msg);
    std::string getErrorMsg();
};
#endif