#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>

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
	KEY_BACKSPACE = 8,
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
		//std::cout << (int)input << std::endl;
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
					//std::cout << "\033[1B" << std::flush;
					break;
				case (int)Action::KEY_UP:
					page->scrollUp();
					//std::cout << "\033[1A" << std::flush;
					break;
				case (int)Action::KEY_LEFT:
					//std::cout << "\033[1D" << std::flush;
					break;
				case (int)Action::KEY_RIGHT:
					//std::cout << "\033[1C" << std::flush;
					break;
				}
			}
		}
		else if ((int)input == (int)Action::KEY_ENTER)
		{
			//term.enterDir();
			//std::cout << "\033[1C" << std::flush;
		}
		term.Draw(page);
	}
	return (0);
}