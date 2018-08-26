#include "file.h"
#include <iomanip>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>

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

File::File(std::string path, std::string dir_entry)
{
    struct stat fileStat;
    if (stat((path+"/"+dir_entry).c_str(), &fileStat))
        perror("Error getting file stat");
        
    this->permission = std::string(permissions(fileStat));
    this->usr_name = getpwuid(fileStat.st_uid)->pw_name;
    this->grp_name = getpwuid(fileStat.st_gid)->pw_name;
    this->size = fileStat.st_size;
    this->last_modified = std::string(strtok(ctime(&fileStat.st_mtime), "\n"));
    this->file_type = (permission[0] == 'd') ? 'd' : 'f';
    this->name = dir_entry;
}