
#include "Terminal.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "fileAttr.h"
#include <pwd.h>
#include <string.h>
#include <string>
#include "TerminalState.h"
#include <iomanip>
#include <algorithm>

//debug
#include <thread> // std::this_thread::sleep_for
#include <chrono>

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

//update Terminal Dimensions
std::pair<int, int> Terminal::updateTermDim()
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    std::pair<int, int> dims(ws.ws_row, ws.ws_col);
    return dims;
}

//constructor
Terminal::Terminal()
{
    std::pair<int, int> temp = updateTermDim();
    this->rows = temp.first;
    this->cols = temp.second;
    if (tcgetattr(0, &(this->orig_term_state)))
    {
        fprintf(stderr, "Error getting current terminal settings");
    }
    char *current_dir = get_current_dir_name();
    this->state_history.push_back(getDirListing(current_dir));
    this->prev_state_index = -1;
    this->curr_state_index = 0;
    this->curr_term_state = this->orig_term_state;
}

//get directory listing
TerminalState Terminal::getDirListing(const char *path)
{
    DIR *dp;
    dp = opendir(path);
    if (dp == NULL)
    {
        perror("Unable to open");
    }
    TerminalState state; //Creating new terminal state
    state.highlight_index = 0;
    state.start_index = 0;
    state.cwd = std::string(path);

    struct dirent *entry;
    while ((entry = readdir(dp)))
    {
        struct stat fileStat;
        stat(entry->d_name, &fileStat);
        fileAttr file(std::string(permissions(fileStat)),
                      getpwuid(fileStat.st_uid)->pw_name,
                      getpwuid(fileStat.st_gid)->pw_name,
                      fileStat.st_size,
                      std::string(strtok(ctime(&fileStat.st_mtime), "\n")),
                      entry->d_name);
        state.files.push_back(file);
        state.files.push_back(file);
        state.files.push_back(file);
        state.files.push_back(file);
        state.files.push_back(file);
    }
    closedir(dp);
    return state;
}

//Draw the current state
void Terminal::Draw()
{
    std::cout << "\033[?1049h"; //New Screen
    std::cout << "\033[0;0H";   //Move cursor to start
    if (curr_state_index >= 0)
    {
        TerminalState &state = this->state_history[curr_state_index];
        int highlight_index = state.highlight_index;
        int end_index = std::min((int)state.files.size(), (int)this->rows + (int)state.start_index - 2);   //-2 as priting dir in the begenning and last row due to cursor
        std::cout << "Current Directory: " << state.cwd + "/";
        for (int i = state.start_index; i < end_index; i++)
        {
            std::cout << std::endl;
            fileAttr file = state.files[i];
            std::cout << file.permission << " ";
            std::cout << file.usr_name << " ";
            std::cout << file.grp_name << " ";
            std::cout << std::right << std::setw(6) << file.size << " ";
            std::cout << file.last_modified;
            if (highlight_index == i)
            {
                std::cout << "\033[30;46m " << file.name << " "
                          << "\033[0m ";
                //std::cout << state.start_index << " " << state.highlight_index << " " << state.files.size();
            }
            else
                std::cout << " " << file.name << " ";
            //std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void Terminal::startEmulation()
{
    Draw();
}

// set non-canon parameters
int Terminal::setParams()
{
    std::cout << "\033[?1049h";         //New Screen
    std::cout << "\033[0;0H";           //Move cursor to start
    if (tcgetattr(0, &orig_term_state)) //get terminal state
    {
        perror("error getting terminal settings");
        return 3;
    }
    curr_term_state = orig_term_state;
    curr_term_state.c_lflag &= ~ICANON;
    curr_term_state.c_lflag &= ~ECHO;
    curr_term_state.c_cc[VMIN] = 1;
    curr_term_state.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &curr_term_state)) //set terminal state
    {
        perror("error setting terminal settings");
        return 3;
    }
    return 0;
}

void Terminal::scrollDown()
{
    TerminalState &state = this->state_history[this->curr_state_index];
    if (state.highlight_index < state.files.size() - 1)
        state.highlight_index += 1;
    if (state.start_index + this->rows - 3 < state.highlight_index)  //-3 as priting dir in the begenning and last row due to cursor
        state.start_index += 1;
    this->Draw();
}

void Terminal::scrollUp()
{
    TerminalState &state = this->state_history[this->curr_state_index];
    if (state.highlight_index >= 1)
        state.highlight_index -= 1;
    if (state.start_index > state.highlight_index)
        state.start_index -= 1;
    this->Draw();
}