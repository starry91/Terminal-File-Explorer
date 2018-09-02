#include "pageManager.h"
#include <unistd.h>
#include <memory>
#include "path.h"

PageManager::PageManager()          //constructor
{
    char *current_dir = get_current_dir_name();
    this->home_dir = std::string(current_dir);
    auto& path = Path::getInstance();
    path.setHomePath(std::string(current_dir));
    this->pageHistory.push_back(std::make_shared<Page>(Page(std::string(current_dir))));
    this->curr_state_index = 0;
}

std::string PageManager::getHomeDir()       //get application HOME 
{
    return this->home_dir;
}

page_Sptr PageManager::getCurrPage()        //get current page being displayed
{
    return this->pageHistory[this->curr_state_index];
}

void PageManager::push(page_Sptr page)      //pushing page to page history list
{
    (this->pageHistory).push_back(page);
    this->curr_state_index += 1;
}

void PageManager::pop()         //popping page when traversing 
{
    (this->pageHistory).pop_back();
}

int PageManager::getCurrStateIndex()        //get index of the current page being displayed
{
    return this->curr_state_index;
}

page_Sptr PageManager::backward()       //going to the previous visited page
{
    if (this->curr_state_index > 0)
        this->curr_state_index--;
    return this->getCurrPage();
}

page_Sptr PageManager::forward()       //getting the next visited page
{
    if (this->curr_state_index < this->pageHistory.size() - 1)
        this->curr_state_index++;
    return this->getCurrPage();
}

void PageManager::updateCurrPage()      //updating the current page 
{
    page_Sptr old_page = this->pageHistory[this->curr_state_index];
    this->pageHistory[this->curr_state_index] = std::make_shared<Page>(Page(std::string(old_page->cwd)));
}