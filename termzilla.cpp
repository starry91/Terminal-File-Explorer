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
#include <unistd.h>
//For non-canonocal mode
#include <termios.h>

//Actions for non-canonical mode
enum class Action
{
	KEY_UP = 65,
	KEY_DOWN = 66,
	KEY_LEFT = 68,
	KEY_RIGHT = 67,
	KEY_ENTER = 13,
	KEY_BACKSPACE = 8,
};

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

//get directory listing
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
					  std::string(strtok(ctime(&fileStat.st_mtime), "\n")),
					  entry->d_name);
		std::cout << file << std::endl;
	}

	closedir(dp);
	return 0;
}

int main()
{
	Terminal term;
	char input;
	std::cout << "\033[?1049h";
	std::cout << "\033[0;0H";
	struct termios curr_term_state, orig_term_state;
	if (tcgetattr(0, &orig_term_state))
	{
		printf("error getting terminal settings");
		return 3;
	}
	curr_term_state = orig_term_state;
	curr_term_state.c_lflag &= ~ICANON;
	curr_term_state.c_lflag &= ~ECHO;
	curr_term_state.c_cc[VMIN] = 1;
	curr_term_state.c_cc[VTIME] = 0;

	if (tcsetattr(0, TCSANOW, &curr_term_state))
	{
		printf("error setting terminal settings");
		return 3;
	}

	std::vector<std::string> files;
	std::cout << "Hello" << std::endl;
	if (listdir(".", files) == 0)
	{
		for (auto it = files.begin(); it != files.end(); it++)
			std::cout << *it << std::endl;
		for (auto it = files.begin(); it != files.end(); it++)
			std::cout << *it << std::endl;
		for (auto it = files.begin(); it != files.end(); it++)
			std::cout << *it << std::endl;
		for (auto it = files.begin(); it != files.end(); it++)
			std::cout << *it << std::endl;
		for (auto it = files.begin(); it != files.end(); it++)
			std::cout << *it << std::endl;
		for (auto it = files.begin(); it != files.end(); it++)
			std::cout << *it << std::endl;
	}
	fflush(stdout);
	while (read(0, &input, 1))
	{
		if (input == '\033')
		{
			read(0, &input, 1);
			if (input == '[')
			{
				read(0, &input, 1);
				switch ((int)input)
				{
				case (int)Action::KEY_DOWN:
					std::cout << "\033[1B" << std::flush;
					break;
				case (int)Action::KEY_UP:
					std::cout << "\033[1A" << std::flush;
					break;
				case (int)Action::KEY_LEFT:
					std::cout << "\033[1D" << std::flush;
					break;
				case (int)Action::KEY_RIGHT:
					std::cout << "\033[1C" << std::flush;
					break;
				}
			}
		}
	}
	return (0);
}