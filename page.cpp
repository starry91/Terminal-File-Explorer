#include "page.h"
#include <dirent.h>
#include <pwd.h>
#include "file.h"
#include <unistd.h>
#include <string>

Page::Page(std::string path = NULL)
{
    if (path.empty())
        path = get_current_dir_name();
    DIR *dp;
    dp = opendir(path.c_str());
    if (dp == NULL)
    {
        perror("Unable to open");
        exit(3);
    }

    struct dirent *entry;
    while ((entry = readdir(dp)))
        this->files.push_back(std::make_shared<File>(File(std::string(entry->d_name))));
    this->highlight_index = 0;
    this->cwd = std::string(path);
}

void Page::scrollDown()
{
    if (this->highlight_index < this->files.size() - 1)
        this->highlight_index += 1;
}

void Page::scrollUp()
{
    if (this->highlight_index >= 1)
        this->highlight_index -= 1;
}

/*
void Page::enterDir()
{
    fileAttr file = state.files[state.highlight_index];
    if (file.file_type == 'd')
    {
        std::string path = state.cwd + "/" + file.name;
        std::cout << "path: " << path << std::endl;
        PageState state = getDirListing((char *)path.c_str());
        this->state_history.push_back(state);
        this->prev_state_index = this->curr_state_index;
        this->curr_state_index = this->curr_state_index + 1;
        this->Draw();
    }
}*/