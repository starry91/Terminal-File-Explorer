//Name: Praveen Balireddy
//Roll: 2018201052

#include "CommandHandler.h"
#include <sys/stat.h>
#include <fstream>
#include "file.h"
#include "page.h"
#include "path.h"
#include <unistd.h>
#include <fstream>
#include "error.h"

void setFilePerms(std::string file, mode_t perms)               //function to set permission to a file
{
    chmod(file.c_str(), perms);
}

void CommandHandler::copyFile(std::string file, std::string dir)            //copy file
{
    std::string dest_file = dir + "/" + File(file).getFileName();
    if (file == dest_file)
    {
        throw Error("cannot copy " + File(file).getFileName() + " to itself");
    }
    std::ifstream input(file, std::ios::binary);
    if (input.fail())
    {
        throw Error("Invalid Args: Cannot find source file ");
    }
    std::ofstream output(dest_file, std::ios::binary);
    if (output.fail())
    {
        throw Error("Invalid Args: Cannot open destination dir");
    }
    std::copy(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(output));
    mode_t perms = File(file).getFilePerms();
    setFilePerms(dest_file, perms);
    input.close();
    output.close();
};

void CommandHandler::copyFiles(const std::vector<std::string> &files)           //copy mutiple files
{
    std::string dest = files[files.size() - 1];
    for (int i = 1; i < files.size() - 1; i++)
    {
        File file = File(files[i]);
        if (file.getFileType() == 'd')
        {
            copyDir(files[i], dest);
        }
        else if (file.getFileName() != "." && file.getFileName() != "..")
        {
            copyFile(files[i], dest);
        }
    }
};

void CommandHandler::copyDir(std::string source_dir, std::string dest_dir)              //copy directory
{
    auto page = std::make_shared<Page>(Page(source_dir));
    File folder = File(source_dir);
    dest_dir = dest_dir + "/" + folder.getFileName();
    struct stat buffer;
    if (stat((dest_dir).c_str(), &buffer) == 0)
    {
        if (source_dir == dest_dir)
            throw Error("source and destination are the same file");
    }
    else if (mkdir(dest_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0)
    {
        throw Error("Directory already exits ");
    }

    for (auto it = page->files.begin(); it != page->files.end(); it++)
    {
        std::string name = (*it)->getFileName();
        if ((*it)->getFileType() == 'd' && name != "." && name != "..")
        {
            copyDir(source_dir + "/" + name, dest_dir);
        }
        else if (name != "." && name != "..")
        {
            copyFile(source_dir + "/" + name, dest_dir);
        }
    }
}

void CommandHandler::delFiles(const std::vector<std::string> &files)                //delete multiple files (used while moving directories)
{
    std::string dest = files[files.size() - 1];
    for (int i = 1; i < files.size() - 1; i++)
    {
        File file = File(files[i]);
        if (file.getFileType() == 'd')
        {
            delDir(files[i]);
        }
        else if (file.getFileName() != "." && file.getFileName() != "..")
        {
            delFile(files[i]);
        }
    }
}

void CommandHandler::delFile(std::string file)                          //delete single file
{
    if (unlink(file.c_str()) < 0)
    {
        throw Error("File does not exist");
    }
}

void CommandHandler::delDir(std::string source_dir)                 //delete directory
{
    try
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
        if (rmdir(source_dir.c_str()) < 0)
        {
            throw Error("Invalid Args: Error deleting dir ");
        };
    }
    catch (Error e)
    {
        throw Error("Directory does not exist");
    }
}

void CommandHandler::rename(std::string old_name, std::string new_name)             //rename file
{
    if (std::rename(old_name.c_str(), new_name.c_str()) < 0)
    {
        throw Error("Source file does not exist");
    }
}

void CommandHandler::createFile(std::string name, std::string dest_dir)             //create file
{
    struct stat buffer;
    if (stat((dest_dir + "/" + name).c_str(), &buffer) == 0)
    {
        throw Error("file " + name + " already exits");
    }
    std::ofstream out((dest_dir + "/" + name));
    if (out.fail())
    {
        throw Error("Destination directory does not exist");
    }
}

void CommandHandler::createDir(std::string name, std::string dest_dir)              //create folder
{
    try
    {
        auto page = std::make_shared<Page>(Page(dest_dir));
    }
    catch (Error e) {
        throw Error("Destination directory does not exist");
    }
    if (mkdir((dest_dir + "/" + name).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0)
    {
        throw Error("Directory already exits ");
    }
}

page_Sptr CommandHandler::goToDir(std::string dir)                  //goto command 
{
    page_Sptr page;
    try
    {
        page = std::make_shared<Page>(Page(dir));
    }
    catch (Error e)
    {
        throw Error("Directory does not exist");
    }
    return page;
}

void CommandHandler::search(std::string name, std::string dir, std::vector<std::string> &output)            //search command
{
    page_Sptr page;
    try
    {
        page = std::make_shared<Page>(Page(dir));
    }
    catch (Error e)
    {
        throw Error("Directory does not exist");
    }
    for (auto it = page->files.begin(); it != page->files.end(); it++)
    {
        std::string abs_path = dir + "/" + (*it)->getFileName();
        if ((*it)->getFileName() == name)
        {
            output.push_back(abs_path);
        }
        if ((*it)->getFileName() != "." && (*it)->getFileName() != ".." && (*it)->getFileType() == 'd')
        {
            search(name, abs_path, output);
        }
    }
}

void CommandHandler::snapshot(std::string dir, std::string file)            //dumping the snapshot
{
    page_Sptr page;
    try
    {
        page = std::make_shared<Page>(Page(dir));
    }
    catch (Error e)
    {
        throw Error("Given Directory does not exist");
    }
    std::ofstream outfile(file, std::ofstream::binary | std::ofstream::out | std::ofstream::app);
    if (outfile.fail())
    {
        throw Error("Cannot create destination file");
    }
    outfile << "." << Path::getInstance().getAppAbsPath(dir) << ":" << std::endl;
    std::vector<std::string> directories;
    for (auto it = page->files.begin(); it != page->files.end(); it++)
    {
        if ((*it)->getFileName() != "." && (*it)->getFileName() != ".." && (*it)->getFileName()[0] != '.')
        {
            outfile << (*it)->getFileName() << " ";
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