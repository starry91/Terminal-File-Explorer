#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <utility>
#include <termios.h>
#include <vector>
#include "page.h"
#include <memory>

enum class Mode
{
	NORMAL = 0,
  COMMAND = 1,
};

class Terminal
{
  public:
    struct termios curr_term_state;
    struct termios orig_term_state;
    Mode mode;

    void DrawView(page_Sptr page);
    void DrawCommand(std::string);
    void scrollDown();
    void scrollUp();
    void enterDir();
    void eraseStatusBar();
    int switchToNormalMode();
    int switchToCommandMode();
};

#endif