
#include "Terminal.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

std::pair<int, int> Terminal::updateTermDim()
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    std::pair<int, int> dims(ws.ws_row, ws.ws_col);
    return dims;
}

Terminal::Terminal()
{
    std::pair<int, int> temp = updateTermDim();
    this->rows = temp.first;
    this->cols = temp.second;
    if (tcgetattr(0, &(this->term_state)))
    {
        printf("Error getting current terminal settings");
    }
}