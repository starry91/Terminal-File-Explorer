#include <sys/ioctl.h>
#include <iostream>
#include <utility>
#include "Terminal.h"
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include "fileAttr.h"
#include <unistd.h>
//For non-canonocal mode
#include <termios.h>
//Terminal State
#include "TerminalState.h"

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
	Terminal term;
	char input;
	term.setParams();	  //set non-canonical params
	term.startEmulation(); //start emulation

	fflush(stdout);
	while (read(0, &input, 1))
	{
		std::cout << (int)input << std::endl;
		if (input == '\033')
		{
			read(0, &input, 1);
			if (input == '[')
			{
				read(0, &input, 1);
				switch ((int)input)
				{
				case (int)Action::KEY_DOWN:
					term.scrollDown();
					//std::cout << "\033[1B" << std::flush;
					break;
				case (int)Action::KEY_UP:
					term.scrollUp();
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
			term.enterDir();
			//std::cout << "\033[1C" << std::flush;
		}
	}
	return (0);
}