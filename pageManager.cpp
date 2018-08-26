#include "pageManager.h"
#include <unistd.h>
#include <memory>

PageManager::PageManager() {
    char *current_dir = get_current_dir_name();
    this->pageHistory.push_back(std::make_shared<Page>(Page(std::string(current_dir))));
    this->curr_state_index = 0;
}

page_Sptr PageManager::getCurrPage() {
    return this->pageHistory[this->curr_state_index];
}