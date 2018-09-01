#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <syslog.h>
#include "Terminal.h"
#include "pageManager.h"
#include <cstring>
#include "CommandHandler.h"
#include "CommandParser.h"
#include "path.h"
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
	syslog(0, "Hello");
	Path &path_obj = Path::getInstance();
	syslog(0, "Main Home: %x", &path_obj);
	Terminal term;
	char input;
	term.switchToNormalMode(); //set non-canonical params
	int search_mode = 0;
	page_Sptr page = pageMgr.getCurrPage();
	term.DrawView(page);
	CommandHandler cmdHandler;
	CommandParser cmdParser;

	fflush(stdout);
	while (read(0, &input, 1))
	{
		if (term.mode == Mode::NORMAL)
		{
			if (search_mode == 0)
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
						if (search_mode == 0)
							page = pageMgr.backward();
						break;
					case (int)Action::KEY_RIGHT:
						if (search_mode == 0)
							page = pageMgr.forward();
						break;
					}
				}
				term.DrawView(page);
			}
			else if ((int)input == (int)Action::KEY_ENTER)
			{
				if (search_mode == 1)
				{
					search_mode = 0;
					term.search_flag = 0;
				}
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
				if (search_mode == 1)
				{
					search_mode = 0;
					term.search_flag = 0;
					page = pageMgr.getCurrPage();
					term.DrawView(page);
				}
				else
				{
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
			}
			else if ((int)input == (int)Action::KEY_h)
			{
				term.search_flag = 0;
				search_mode = 0;
				page_Sptr new_page = page->gotoHome(pageMgr.getHomeDir());
				int curr_index = pageMgr.getCurrStateIndex();
				while (pageMgr.pageHistory.size() > curr_index + 1)
					pageMgr.pop();
				pageMgr.push(new_page);
				page = pageMgr.getCurrPage();
				term.DrawView(page);
			}
			else if (search_mode == 0 && (int)input == (int)Action::KEY_COLON)
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
						else if (start > 0)
						{
							buffer[start - 1] = 0;
							start--;
						}
						term.DrawCommand(std::string(buffer));
					}
					else if (input == '\n')
					{
						syslog(0, "Buffer: [%s]", buffer);
						std::vector<std::string> command_args = cmdParser.getArgs(std::string(buffer));
						std::vector<std::string> translated_args = cmdParser.translateArgs(command_args, page);
						for (auto it = translated_args.begin(); it != translated_args.end(); it++)
							syslog(0, "Arg: [%s]", (*it).c_str());
						if (translated_args[0] == "copy")
						{
							syslog(0, "Copy Command Entered");
							cmdHandler.copyFiles(translated_args);
						}
						else if (translated_args[0] == "move")
						{
							syslog(0, "Move Command Entered");
							cmdHandler.copyFiles(translated_args);
							cmdHandler.delFiles(translated_args);
							pageMgr.updateCurrPage();
							page = pageMgr.getCurrPage();
							term.DrawView(page);
						}
						else if (translated_args[0] == "rename")
						{
							cmdHandler.rename(translated_args[1], translated_args[2]);
							pageMgr.updateCurrPage();
							page = pageMgr.getCurrPage();
							term.DrawView(page);
						}
						else if (translated_args[0] == "create_file")
						{
							cmdHandler.createFile(translated_args[1], translated_args[2]);
							pageMgr.updateCurrPage();
							page = pageMgr.getCurrPage();
							term.DrawView(page);
						}
						else if (translated_args[0] == "create_dir")
						{
							cmdHandler.createDir(translated_args[1], translated_args[2]);
							pageMgr.updateCurrPage();
							page = pageMgr.getCurrPage();
							term.DrawView(page);
						}
						else if (translated_args[0] == "delete_file")
						{
							cmdHandler.delFile(translated_args[1]);
							pageMgr.updateCurrPage();
							page = pageMgr.getCurrPage();
							term.DrawView(page);
						}
						else if (translated_args[0] == "delete_dir")
						{
							cmdHandler.delDir(translated_args[1]);
							pageMgr.updateCurrPage();
							page = pageMgr.getCurrPage();
							term.DrawView(page);
						}
						else if (translated_args[0] == "goto")
						{
							auto new_page = cmdHandler.goToDir(translated_args[1]);
							int curr_index = pageMgr.getCurrStateIndex();
							while (pageMgr.pageHistory.size() > curr_index + 1)
								pageMgr.pop();
							pageMgr.push(new_page);
							page = pageMgr.getCurrPage();
							term.DrawView(page);
						}
						else if (translated_args[0] == "search")
						{
							std::vector<std::string> search_output;
							cmdHandler.search(command_args[1], Path::getInstance().getSystemAbsPath("."), search_output);
							page = std::make_shared<Page>(Page(search_output));
							term.search_flag = 1;
							search_mode = 1;
							term.DrawView(page);
							break;
							for (auto it = search_output.begin(); it != search_output.end(); it++)
								syslog(0, "search out: [%s]", (*it).c_str());
						}
						else if (translated_args[0] == "snapshot")
						{
							syslog(0, "Snapshot dir:out %s", translated_args[1].c_str());
							cmdHandler.snapshot(translated_args[1], translated_args[2]);
						}
						term.eraseStatusBar();
						term.DrawCommand("");
						std::memset(buffer, 0, 1024);
						start = 0;
					}
					else if ((int)input == (int)Action::KEY_ESC)
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