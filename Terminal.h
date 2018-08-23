#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <utility>

class Terminal{
    protected:
        int rows;
        int cols;
    public:
        Terminal();
        std::pair<int,int> updateTermDim();
        int getRows();
        int getCols();
};

#endif