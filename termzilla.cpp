#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <syslog.h>
#include "Terminal.h"
#include "pageManager.h"
#include <cstring>
//Actions for non-canonical mode
enum class Action
{
	KEY_UP = 65,
	KEY_DOWN = 66,
	KEY_LEFT = 68,
	KEY_RIGHT = 67,
	KEY_ENTER = 10,
	KEY_BACKSPACE = 127,
	KEY_h = 104,
	KEY_COLON = 58,
	KEY_ESC = 27,
};

int main()
{
	PageManager pageMgr;
	Terminal term;
	char input;
	term.switchToNormalMode(); //set non-canonical params

	page_Sptr page = pageMgr.getCurrPage();
	term.DrawView(page);

	fflush(stdout);
	while (read(0, &input, 1))
	{
		if (term.mode == Mode::NORMAL)
		{
			page_Sptr page = pageMgr.getCurrPage();
			syslog(0, "Input: %d", (int)input);
			if (input == '\033')
			{
				read(0, &input, 1);
				if (input == '[')
				{
					read(0, &input, 1);
					switch ((int)input)
					{
					case (int)Action::KEY_DOWN:
						page->scrollDown();
						break;
					case (int)Action::KEY_UP:
						page->scrollUp();
						break;
					case (int)Action::KEY_LEFT:
						page = pageMgr.backward();
						break;
					case (int)Action::KEY_RIGHT:
						page = pageMgr.forward();
						break;
					}
				}
				term.DrawView(page);
			}
			else if ((int)input == (int)Action::KEY_ENTER)
			{
				page_Sptr new_page = page->enterDir();
				if (new_page != NULL) //if opening a folder
				{
					int curr_index = pageMgr.getCurrStateIndex();
					while (pageMgr.pageHistory.size() > curr_index + 1)
						pageMgr.pop();
					pageMgr.push(new_page);
					page = pageMgr.getCurrPage();
					term.DrawView(page);
				}
				//std::cout << "\033[1C" << std::flush;
			}
			else if ((int)input == (int)Action::KEY_BACKSPACE)
			{
				//syslog(0, "Hello Input: %d", (int)input);
				page_Sptr new_page = page->gotoParent();
				if (new_page != NULL) //if opening a folder
				{
					int curr_index = pageMgr.getCurrStateIndex();
					while (pageMgr.pageHistory.size() > curr_index + 1)
						pageMgr.pop();
					pageMgr.push(new_page);
					page = pageMgr.getCurrPage();
					term.DrawView(page);
				}
			}
			else if ((int)input == (int)Action::KEY_h)
			{
				page_Sptr new_page = page->gotoHome(pageMgr.getHomeDir());
				int curr_index = pageMgr.getCurrStateIndex();
				while (pageMgr.pageHistory.size() > curr_index + 1)
					pageMgr.pop();
				pageMgr.push(new_page);
				page = pageMgr.getCurrPage();
				term.DrawView(page);
			}
			else if ((int)input == (int)Action::KEY_COLON)
			{
				term.DrawCommand("");
				char buffer[1024];
				std::memset(buffer, 0, 1024);
				char input;
				int start = 0;
				while (true)
				{
					syslog(0, "Testing");
					input = getchar();
					syslog(0, "Input: %d %d", input, '\n');
					if (input != '\n' && ((int)input != (int)Action::KEY_ESC))
					{
						if ((int)input != (int)Action::KEY_BACKSPACE)
						{
							buffer[start] = input;
							start++;
							syslog(0, "Hello Input: %s", buffer);
						}
						else if(start > 0)
						{
							buffer[start - 1] = 0;
							start--;
						}
						term.DrawCommand(std::string(buffer));
					}
					else if (input == '\n')
					{
						syslog(0, "Returning command");
						term.eraseStatusBar();
						term.DrawCommand("");
						std::memset(buffer, 0, 1024);
						start = 0;
					}
					else
						break;
				}
				term.DrawView(page);
			}
		}
		else
		{
		}
		//std::cout << "\033[1C" << std::flush;
	}
	return (0);
}