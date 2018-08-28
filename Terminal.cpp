
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <iomanip>
#include <string>
#include "Terminal.h"
#include "page.h"
#include "path.h"
//debug
#include <thread> // std::this_thread::sleep_for
#include <chrono>
#include <syslog.h>
//Draw the current state
void Terminal::DrawView(page_Sptr page)
{
    std::cout << "\e[2J" << std::flush;
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    int rows = ws.ws_row;
    int col = ws.ws_col;

    int highlight_index = page->highlight_index;
    int begin = (highlight_index > rows - 3) ? (highlight_index - rows + 3) : 0;
    int offset = (rows - 2 > (page->files.size() - begin)) ? page->files.size() - begin : rows - 2;

    std::cout << "\033[1;1H"; //Move cursor to start
    std::cout << "Current Directory: " << Path::getAppAbsPath(page->cwd + "/");
    int cursor_row = 2;
    for (int i = begin; i < begin + offset; i++)
    {
        std::cout << "\033[" << cursor_row << ";0H";
        std::cout << page->files[i]->permission << " ";
        std::cout << page->files[i]->usr_name << " ";
        std::cout << page->files[i]->grp_name << " ";
        std::cout << std::right << std::setw(6) << page->files[i]->size << " ";
        std::cout << page->files[i]->last_modified;
        if (highlight_index == i)
        {
            std::cout << "\033[30;46m " << page->files[i]->name << " "
                      << "\033[0m ";
            //std::cout << state.start_index << " " << state.highlight_index << " " << state.files.size();
        }
        else
        {
            if (page->files[i]->file_type == 'd')
                std::cout << "\033[35;10m " << page->files[i]->name << " \033[0m";
            else
                std::cout << " " << page->files[i]->name << " ";
        }
        std::cout << std::flush;
        cursor_row += 1;
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    syslog(0, "Mode: %d Rows: %d", (int)mode, rows);
    if (this->mode == Mode::COMMAND)
        std::cout << "\033[" << rows << ";0H"
                  << ":" << std::flush;
}

void Terminal::DrawCommand(std::string cmd)
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    int rows = ws.ws_row;
    int col = ws.ws_col;
    this->eraseStatusBar();
    std::cout << ":" << cmd << std::flush;
}

void Terminal::eraseStatusBar()
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    int rows = ws.ws_row;
    int col = ws.ws_col;
    std::cout << "\033[" << rows << ";0H"
              << "\033[K" << std::flush;
}

// set non-canon parameters
int Terminal::switchToNormalMode()
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
    this->mode = Mode::NORMAL;
    return 0;
}

int Terminal::switchToCommandMode()
{
    curr_term_state = orig_term_state;

    if (tcsetattr(0, TCSANOW, &curr_term_state)) //set terminal state
    {
        perror("error setting terminal settings");
        return 3;
    }
    this->mode = Mode::COMMAND;
    return 0;
}