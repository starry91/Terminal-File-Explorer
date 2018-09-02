#include "file.h"
#include <iomanip>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <vector>
#include "error.h"
//find permissions
char *permissions(struct stat st)
{
    char *mode = (char *)malloc(sizeof(char) * 10 + 1);
    mode_t perm = st.st_mode;
    mode[0] = (perm & S_IFDIR) ? 'd' : '-';
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
    return mode;
}

File::File(std::string path, int search_mode)
{
    this->dir_entry = path;
}

File::File(std::string file)
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
    if (stat(file.c_str(), &fileStat))
    {
        throw Error("Invalid Args: Cannot find file");
    }
}

File::File(std::string path, std::string dir_entry)
{
    this->dir_entry = dir_entry;
    if (stat((path + "/" + dir_entry).c_str(), &fileStat))
    {
        throw Error("Invalid Args: Cannot find file");
    }
}

std::string File::getGroupName()
{
    return std::string(getpwuid(fileStat.st_gid)->pw_name);
}
std::string File::getPermission()
{
    char *mode = (char *)malloc(sizeof(char) * 10 + 1);
    mode_t perm = this->fileStat.st_mode;
    mode[0] = (perm & S_IFDIR) ? 'd' : '-';
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
    return (this->getPermission().c_str()[0] == 'd') ? 'd' : 'f';
}
std::string File::getFileName()
{
    return this->dir_entry;
}

std::string File::getUserName()
{
    return std::string(getpwuid(this->fileStat.st_uid)->pw_name);
}

mode_t File::getFilePerms()
{
    return this->fileStat.st_mode;
}