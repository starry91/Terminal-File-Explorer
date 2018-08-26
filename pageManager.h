#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <vector>
#include "page.h"
#include <memory>


class PageManager
{
  public:
    std::vector<page_Sptr> pageHistory;
    int curr_state_index;

    page_Sptr getCurrPage();
    page_Sptr backward();
    page_Sptr forward();
    void push(page_Sptr);
    PageManager();
};


#endif