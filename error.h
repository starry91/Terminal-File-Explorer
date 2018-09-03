//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef Error_H
#define Error_H

#include <string>
#include <iostream>

//Error class to handle errors

class Error
{
    std::string msg;
    public:
    Error(std::string msg);
    std::string getErrorMsg();
};
#endif