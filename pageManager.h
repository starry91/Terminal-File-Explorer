#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <vector>
#include "page.h"
#include <memory>
#include <string>

class PageManager
{
  public:
    std::vector<page_Sptr> pageHistory;
    int curr_state_index;
    std::string home_dir;

    page_Sptr getCurrPage();
    page_Sptr backward();
    page_Sptr forward();
    void push(page_Sptr);
    void pop();
    int getCurrStateIndex();
    std::string getHomeDir();
    void updateCurrPage();
    PageManager();
};


#endif