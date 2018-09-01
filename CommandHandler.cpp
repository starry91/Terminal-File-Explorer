#include "CommandHandler.h"
#include <sys/stat.h>
#include <syslog.h>
#include <fstream>
#include "file.h"
#include "page.h"
#include "path.h"
#include <unistd.h>
#include <fstream>

void setFilePerms(std::string file, mode_t perms)
{
    chmod(file.c_str(), perms);
}

void CommandHandler::copyFile(std::string file, std::string dir)
{

    // FILE *from, *to;
    // char ch;
    std::string dest_file = dir + "/" + File(file).getFileName();
    syslog(0, "FileName: %s", File(file).getFileName().c_str());
    syslog(0, "Source File: %s", file.c_str());
    syslog(0, "Dest File: %s", dest_file.c_str());
    std::ifstream input(file, std::ios::binary);
    std::ofstream output(dest_file, std::ios::binary);
    std::copy(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(output));
    mode_t perms = File(file).getFilePerms();
    setFilePerms(dest_file, perms);
};

void CommandHandler::copyFiles(const std::vector<std::string> &files)
{
    std::string dest = files[files.size() - 1];
    for (int i = 1; i < files.size() - 1; i++)
    {
        File file = File(files[i]);
        if (file.getFileType() == 'd')
        {
            syslog(0, "In copy file/ copy dir: %s", files[i].c_str());
            copyDir(files[i], dest);
        }
        else if (file.getFileName() != "." && file.getFileName() != "..")
        {
            syslog(0, "In copy file/ copy file: %s", files[i].c_str());
            copyFile(files[i], dest);
        }
    }
};

void CommandHandler::copyDir(std::string source_dir, std::string dest_dir)
{
    auto page = std::make_shared<Page>(Page(source_dir));
    File folder = File(source_dir);
    dest_dir = dest_dir + "/" + folder.getFileName();
    syslog(0, "Make Dir: %s", dest_dir.c_str());
    mkdir(dest_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    for (auto it = page->files.begin(); it != page->files.end(); it++)
    {
        std::string name = (*it)->getFileName();
        syslog(0, "FileType Dir: %s", (source_dir + "/" + name).c_str());
        if ((*it)->getFileType() == 'd' && name != "." && name != "..")
        {
            syslog(0, "Copy Dir: %s", (source_dir + "/" + name).c_str());
            //mkdir((dest_dir + "/" + name).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            syslog(0, "Created Dir: %s", (source_dir + "/" + name).c_str());
            copyDir(source_dir + "/" + name, dest_dir);
            syslog(0, "Directory: %s copied", (source_dir + "/" + name).c_str());
        }
        else if (name != "." && name != "..")
        {
            copyFile(source_dir + "/" + name, dest_dir);
        }
    }
}

void CommandHandler::delFiles(const std::vector<std::string> &files)
{
    std::string dest = files[files.size() - 1];
    for (int i = 1; i < files.size() - 1; i++)
    {
        File file = File(files[i]);
        if (file.getFileType() == 'd')
        {
            //syslog(0, "In copy file/ copy dir: %s", files[i].c_str());
            delDir(files[i]);
        }
        else if (file.getFileName() != "." && file.getFileName() != "..")
        {
            //syslog(0, "In copy file/ copy file: %s", files[i].c_str());
            delFile(files[i]);
        }
    }
}

void CommandHandler::delFile(std::string file)
{
    unlink(file.c_str());
}

void CommandHandler::delDir(std::string source_dir)
{
    auto page = std::make_shared<Page>(Page(source_dir));
    File folder = File(source_dir);
    for (auto it = page->files.begin(); it != page->files.end(); it++)
    {
        std::string name = (*it)->getFileName();
        if ((*it)->getFileType() == 'd' && name != "." && name != "..")
        {
            delDir(source_dir + "/" + name);
        }
        else if (name != "." && name != "..")
        {
            delFile(source_dir + "/" + name);
        }
    }
    rmdir(source_dir.c_str());
}

void CommandHandler::rename(std::string old_name, std::string new_name)
{
    syslog(0, "Rename old %s", old_name.c_str());
    syslog(0, "Rename new %s", new_name.c_str());
    std::rename(old_name.c_str(), new_name.c_str());
}

void CommandHandler::createFile(std::string name, std::string dest_dir)
{
    //if (File(name).getFileType() != 'd')
    {
        std::ofstream out((dest_dir + "/" + File(name).getFileName()));
    }
}

void CommandHandler::createDir(std::string name, std::string dest_dir)
{
    mkdir((dest_dir + "/" + File(name).getFileName()).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

page_Sptr CommandHandler::goToDir(std::string dir)
{
    return std::make_shared<Page>(Page(dir));
}

void CommandHandler::search(std::string name, std::string dir, std::vector<std::string> &output)
{
    syslog(0, "In search search path: %s", dir.c_str());
    auto page = std::make_shared<Page>(Page(dir));
    for (auto it = page->files.begin(); it != page->files.end(); it++)
    {
        std::string abs_path;
        syslog(0, "In search search file path %s", abs_path.c_str());
        if ((*it)->getFileName() == name)
        {
            abs_path = dir;
            output.push_back(abs_path);
        }
        else if ((*it)->getFileName() != "." && (*it)->getFileName() != ".." && (*it)->getFileType() == 'd')
        {
            abs_path = dir + "/" + (*it)->getFileName();
            search(name, abs_path, output);
        }
    }
}

void CommandHandler::snapshot(std::string dir, std::string file)
{
    syslog(0, "Snapshot: dir %s  file %s", dir.c_str(), file.c_str());
    auto page = std::make_shared<Page>(Page(dir));
    std::ofstream outfile(file, std::ofstream::binary | std::ofstream::out | std::ofstream::app);
    outfile << Path::getInstance().getAppAbsPath(dir) << ":" << std::endl;
    std::vector<std::string> directories;
    for (auto it = page->files.begin(); it != page->files.end(); it++)
    {
        if ((*it)->getFileName() != "." && (*it)->getFileName() != ".." && (*it)->getFileName()[0] != '.')
        {   
            outfile << (*it)->getFileName() << " ";
            //syslog(0, "In search search file path %s", abs_path.c_str());
            if ((*it)->getFileType() == 'd')
            {
                std::string abs_path = dir + "/" + (*it)->getFileName();
                directories.push_back(abs_path);
            }
        }
    }
    outfile << "\n";
    outfile << "\n";
    outfile.close();
    for (auto it = directories.begin(); it != directories.end(); it++)
    {
        snapshot(*it, file);
    }
}