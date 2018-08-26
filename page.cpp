#include "page.h"
#include <dirent.h>
#include <pwd.h>
#include "file.h"
#include <unistd.h>
#include <string>
#include <memory>
#include "path.h"
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
        this->files.push_back(std::make_shared<File>(File(path, std::string(entry->d_name))));
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


page_Sptr Page::enterDir()
{
    auto file = this->files[this->highlight_index];
    if (file->file_type == 'd')
    {
        std::string path;
        if(file->name == "..")
            path = Path::getParentDir(this->cwd);
        else if(file->name == ".")
            path = this->cwd;
            else path = this->cwd + "/" + file->name;
        return std::make_shared<Page>(Page((char *)path.c_str()));
    }
    else {
        perror("Not a directory");
    }
}