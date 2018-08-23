#include <sys/ioctl.h>
#include <iostream>
#include <utility>
#include "Terminal.h"
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include "fileAttr.h"

enum class Action
{
	KEY_UP = 72,
	KEY_DOWN = 80,
	KEY_LEFT = 75,
	KEY_RIGHT = 77,
	KEY_ENTER = 13,
	KEY_BACKSPACE = 8,
};

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

int listdir(const char *path, std::vector<std::string> &out)
{
	struct dirent *entry;
	DIR *dp;
	struct stat fileStat;
	dp = opendir(path);
	if (dp == NULL)
	{
		perror("opendir");
		return -1;
	}

	while ((entry = readdir(dp)))
	{
		stat(entry->d_name, &fileStat);
		fileAttr file(std::string(permissions(fileStat)),
				getpwuid(fileStat.st_uid)->pw_name,
				getpwuid(fileStat.st_gid)->pw_name,
				fileStat.st_size,
				std::string(strtok(ctime(&fileStat.st_mtime),"\n")),
				entry->d_name);
		std::cout << file << std::endl;
	}

	closedir(dp);
	return 0;
}

int main()
{
	Terminal term;
	std::cout << "\033[?1049h";
	std::cout << "\033[0;0H";
	std::vector<std::string> files;
	std::cout << "Hello" << std::endl;
	//std::cout << listdir(".", files) << std::endl;
	if (listdir(".", files) == 0)
	{
		for (auto it = files.begin(); it != files.end(); it++)
			std::cout << *it << std::endl;
	}
	return (0);
}