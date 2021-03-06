//Name: Praveen Balireddy
//Roll: 2018201052

#include "file.h"
#include <iomanip>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <vector>
#include "error.h"
#include <grp.h>

File::File(std::string path, int search_mode) //constructor for search mode
{
    this->dir_entry = path;
}

File::File(std::string file) //default constructor
{
    std::vector<std::string> tokens;
    int prev = 0;
    for (int i = 0; i <= file.length(); i++)
    {
        if (file[i] == '/' || i == file.length())
        {
            tokens.push_back(file.substr(prev, i - prev));
            prev = i + 1;
        }
    }
    this->dir_entry = tokens[tokens.size() - 1];
    if (lstat(file.c_str(), &fileStat))
    {
        throw Error("Invalid Args: Cannot find file");
    }
    if (stat(file.c_str(), &effFileStat))
    {
        effFileStat = fileStat;
    }
}

File::File(std::string file, char stat_type) //to get stat in search mode
{
    std::vector<std::string> tokens;
    int prev = 0;
    for (int i = 0; i <= file.length(); i++)
    {
        if (file[i] == '/' || i == file.length())
        {
            tokens.push_back(file.substr(prev, i - prev));
            prev = i + 1;
        }
    }
    this->dir_entry = tokens[tokens.size() - 1];
    if (lstat(file.c_str(), &fileStat))
    {
        throw Error("Invalid Args: Cannot find file");
    }
    if (stat(file.c_str(), &effFileStat))
    {
        effFileStat = fileStat;
    }
}

char File::getEffFileType()
{
    mode_t perm = this->effFileStat.st_mode;
    return S_ISDIR(perm) ? 'd' : S_ISLNK(perm) ? 'l' : 'f';
}

File::File(std::string path, std::string dir_entry) //default constructor when creating first page in Page manager constructor
{
    this->dir_entry = dir_entry;
    if (lstat((path + "/" + dir_entry).c_str(), &fileStat))
    {
        throw Error("lstat Invalid Args: Cannot find file");
    }
    if (stat((path + "/" + dir_entry).c_str(), &effFileStat))
    {
        effFileStat = fileStat;
    }
}

std::string File::getGroupName() //getting file attributes
{
    if (getgrgid(fileStat.st_gid) == NULL)
        return std::to_string(fileStat.st_gid);
    return std::string(getgrgid(fileStat.st_gid)->gr_name);
}
std::string File::getPermission()
{
    char *mode = (char *)malloc(sizeof(char) * 10 + 1);
    mode_t perm = this->fileStat.st_mode;
    mode[0] = ((perm & S_IFMT) == S_IFDIR) ? 'd' : ((perm & S_IFMT) == S_IFLNK ? 'l' : ((perm & S_IFMT) == S_IFCHR ? 'c' : ((perm & S_IFMT) == S_IFBLK ? 'b' : ((perm & S_IFMT) == S_IFIFO ? 'p' : ((perm & S_IFMT) == S_IFSOCK ? 'p' : ((perm & S_IFMT) == S_IFREG ? '-' : 'u'))))));
    mode[1] = (perm & S_IRUSR) ? 'r' : '-';
    mode[2] = (perm & S_IWUSR) ? 'w' : '-';
    mode[3] = (perm & S_IXUSR) ? 'x' : '-';
    mode[4] = (perm & S_IRGRP) ? 'r' : '-';
    mode[5] = (perm & S_IWGRP) ? 'w' : '-';
    mode[6] = (perm & S_IXGRP) ? 'x' : '-';
    mode[7] = (perm & S_IROTH) ? 'r' : '-';
    mode[8] = (perm & S_IWOTH) ? 'w' : '-';
    mode[9] = (perm & S_IXOTH) ? 'x' : '-';
    mode[10] = '\0';
    return std::string(mode);
}
size_t File::getSize()
{
    return fileStat.st_size;
}
std::string File::getLastModified()
{
    return std::string(strtok(ctime(&fileStat.st_mtime), "\n"));
}
char File::getFileType()
{
    return (this->getPermission().c_str()[0] == 'd') ? 'd' : ((this->getPermission().c_str()[0] == 'f') ? 'f' : 'l');
}
std::string File::getFileName()
{
    return this->dir_entry;
}

std::string File::getUserName()
{
    if (getpwuid(fileStat.st_uid) == NULL)
        return std::to_string(fileStat.st_uid);
    return std::string(getpwuid(this->fileStat.st_uid)->pw_name);
}

mode_t File::getFilePerms()
{
    return this->fileStat.st_mode;
}