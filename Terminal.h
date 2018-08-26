#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <utility>
#include <termios.h>
#include <vector>
#include "page.h"
#include <memory>

class Terminal
{
  public:
    struct termios curr_term_state;
    struct termios orig_term_state;

    void Draw(page_Sptr page);
    void scrollDown();
    void scrollUp();
    void enterDir();
    int setParams();
};

#endif