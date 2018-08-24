#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <utility>
#include <termios.h>

class Terminal
{
  public:
    int rows;
    int cols;
    struct termios term_state;
    Terminal();
    std::pair<int, int> updateTermDim();

};

#endif