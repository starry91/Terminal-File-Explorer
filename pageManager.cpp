#include "pageManager.h"
#include <unistd.h>
#include <memory>
#include <syslog.h>
#include "path.h"

namespace Path{
    std::string HOME_APPLICATION;
}

PageManager::PageManager()
{
    char *current_dir = get_current_dir_name();
    this->home_dir = std::string(current_dir);
    syslog(0, "PageMGr/ Current Dir: %s",current_dir);
    Path::HOME_APPLICATION = std::string(current_dir);
    this->pageHistory.push_back(std::make_shared<Page>(Page(std::string(current_dir))));
    this->curr_state_index = 0;
}

std::string PageManager::getHomeDir(){
    return this->home_dir;
}

page_Sptr PageManager::getCurrPage()
{
    return this->pageHistory[this->curr_state_index];
}

void PageManager::push(page_Sptr page)
{
    (this->pageHistory).push_back(page);
    this->curr_state_index += 1;
    //syslog(0, "Pushing page: %s, %d",page->cwd.c_str(),this->curr_state_index);
}

void PageManager::pop()
{
    (this->pageHistory).pop_back();
}

int PageManager::getCurrStateIndex()
{
    return this->curr_state_index;
}

page_Sptr PageManager::backward()
{
    if (this->curr_state_index > 0)
        this->curr_state_index--;
    //syslog(0, "Going back: %d", this->curr_state_index);
    return this->getCurrPage();
}

page_Sptr PageManager::forward()
{
    if (this->curr_state_index < this->pageHistory.size()-1)
        this->curr_state_index++;
    return this->getCurrPage();
}