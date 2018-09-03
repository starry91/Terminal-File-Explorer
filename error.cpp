//Name: Praveen Balireddy
//Roll: 2018201052

#include "error.h"

Error::Error(std::string msg)
{
    this->msg = msg;
}

std::string Error::getErrorMsg()
{
    return this->msg;
}