#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <syslog.h>
#include "Terminal.h"
#include "pageManager.h"

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
};

int main()
{
	PageManager pageMgr;
	Terminal term;
	char input;
	term.setParams(); //set non-canonical params

	page_Sptr page = pageMgr.getCurrPage();
	term.Draw(page);

	fflush(stdout);
	while (read(0, &input, 1))
	{
		page_Sptr page = pageMgr.getCurrPage();
		//syslog(0, "Input: %d", (int)input);
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
			term.Draw(page);
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
				term.Draw(page);
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
				term.Draw(page);
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
			term.Draw(page);
		}
		//std::cout << "\033[1C" << std::flush;
	}
	return (0);
}