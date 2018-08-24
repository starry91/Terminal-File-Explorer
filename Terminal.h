#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <utility>
#include <termios.h>
#include <vector>
#include "TerminalState.h"
#include <memory>

class Terminal
{
  public:
    int rows;
    int cols;
    struct termios curr_term_state;
    struct termios orig_term_state;
    std::vector<TerminalState> state_history;
    int prev_state_index;
    int curr_state_index;

    Terminal();
    std::pair<int, int> updateTermDim();
    TerminalState getDirListing(const char *);
    void Draw();
    void startEmulation();
    void scrollDown();
    void scrollUp();
    void enterDir();
    int setParams();
};

#endif