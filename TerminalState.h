#ifndef TERMINALSTATE_H
#define TERMINALSTATE_H

#include <vector>
#include <string>
#include "fileAttr.h"
class TerminalState
{
  public:
    std::vector<fileAttr> files;
    int highlight_index;
    int start_index;
    std::string cwd;
};

#endif