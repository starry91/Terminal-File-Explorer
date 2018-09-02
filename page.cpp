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
#include "error.h"
Page::Page(std::string path = NULL)
{
    if (path.empty())
        path = get_current_dir_name();
    DIR *dp;
    dp = opendir(path.c_str());
    if (dp == NULL)
    {
        throw Error("Invalid Args: " + Path::getInstance().getAppAbsPath(path) + " does not exist");
    }
    //syslog(0, "Page constructor: %s", path.c_str());
    struct dirent *entry;
    while ((entry = readdir(dp)))
        this->files.push_back(std::make_shared<File>(File(path, std::string(entry->d_name))));
    this->highlight_index = 0;
    this->cwd = std::string(path);
}

Page::Page(std::vector<std::string> files)
{
    for (auto it = files.begin(); it != files.end(); it++)
    {
        this->files.push_back(std::make_shared<File>(*it, 0));
        this->highlight_index = 0;
    }
    this->cwd = "search";
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
    auto path_obj = Path::getInstance();
    std::string search_path = path_obj.getSystemAbsPath(file->getFileName());
    syslog(0, "Enter Dir: [%s] file type: [%c]", search_path.c_str(), File(search_path).getFileType());
    if (this->cwd == "search")
    {
        if (File(search_path,'n').getFileType() == 'd')
        {
            syslog(0, "Opening searh page: %s", search_path.c_str());
            return std::make_shared<Page>(Page((char *)search_path.c_str()));
        }
        else //if (File(search_path).getFileType() == 'f')
        {
            int pid = fork();
            if (pid == 0)
            { //child
                //char * const args[3] = {"xdg-open", (char*) (this->cwd + "/" + file->name).c_str(), NULL};
                //execvp ("/usr/bin/xdg-open", args);
                execl("/usr/bin/xdg-open", "xdg-open", search_path.c_str(), NULL);
            }
            else
            {
                int err = waitpid(-1, NULL, WUNTRACED);
                syslog(0, "Waiting: %d", err);
            }
        }
    }
    else if (file->getFileType() == 'd')
    {
        std::string path;
        if (file->getFileName() == "..")
            path = path_obj.getParentDir(this->cwd);
        else if (file->getFileName() == ".")
            path = this->cwd;
        else
            path = this->cwd + "/" + file->getFileName();
        //syslog(0, "Waiting: %s", path);
        if (path.length() >= path_obj.getHomePath().length())
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
            std::string path = this->cwd + "/" + file->getFileName();
            execl("/usr/bin/xdg-open", "xdg-open", path.c_str(), NULL);
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
    auto path_obj = Path::getInstance();
    std::string path = path_obj.getParentDir(this->cwd);
    if (path.length() >= path_obj.getHomePath().length())
        return std::make_shared<Page>(Page((char *)path.c_str()));
    return NULL;
}

page_Sptr Page::gotoHome(std::string path)
{
    auto path_obj = Path::getInstance();
    return std::make_shared<Page>(Page((char *)path_obj.getHomePath().c_str()));
}