#include <sys/ioctl.h>
#include <iostream>
#include <utility>
#include "Terminal.h"
#include <string>
#include <chrono>
#include <thread>

enum class Action
{
	KEY_UP = 72,
	KEY_DOWN = 80,
	KEY_LEFT = 75,
	KEY_RIGHT = 77,
	KEY_ENTER = 13,
	KEY_BACKSPACE = 8,
};

int main()
{
	std::string x = "hello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\nhello\n";
	Terminal term;
	std::cout << "\033[?1049h";
	std::cout << "\033[0;0H";
	std::cout << term.getRows() << " " << term.getCols() << std::endl;
	std::cout << x << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << x << std::endl;
	return (0);
}