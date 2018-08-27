#include "page.h"
#include <dirent.h>
#include <pwd.h>
#include "file.h"
#include <unistd.h>
#include <string>
#include <memory>
#include "path.h"
#include <sys/wait.h>
#include <syslog.h>

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
        if (file->name == "..")
            path = Path::getParentDir(this->cwd);
        else if (file->name == ".")
            path = this->cwd;
        else
            path = this->cwd + "/" + file->name;
        if (path.length() >= Path::HOME_APPLICATION.length())
            return std::make_shared<Page>(Page((char *)path.c_str()));
        //syslog(0, "Waiting: %s", path);
    }
    else
    {
        int pid = fork();
        if (pid == 0)
        { //child
            //char * const args[3] = {"xdg-open", (char*) (this->cwd + "/" + file->name).c_str(), NULL};
            //execvp ("/usr/bin/xdg-open", args);
            execl("/usr/bin/xdg-open", "xdg-open", file->name.c_str(), NULL);
        }
        else
        {
            int err = waitpid(-1, NULL, WUNTRACED);
            syslog(0, "Waiting: %d", err);
        }
    }
    return NULL;
}

page_Sptr Page::gotoParent()
{
    std::string path = Path::getParentDir(this->cwd);
    if (path.length() >= Path::HOME_APPLICATION.length())
        return std::make_shared<Page>(Page((char *)path.c_str()));
    return NULL;
}

page_Sptr Page::gotoHome(std::string path)
{
    return std::make_shared<Page>(Page((char *)Path::HOME_APPLICATION.c_str()));
}