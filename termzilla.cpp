//Name: Praveen Balireddy
//Roll: 2018201052

#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "Terminal.h"
#include "pageManager.h"
#include <cstring>
#include "CommandHandler.h"
#include "CommandParser.h"
#include "path.h"
#include "error.h"
#include <syslog.h>
//Actions for normal mode
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
	KEY_q = 113,
};

int main()
{
	PageManager pageMgr;				  //page manager to manage all the views
	Path &path_obj = Path::getInstance(); //singleton path class to translate paths to system absolute paths and vice versa
	Terminal term;						  //terminal class - to draw different views
	char input[10];
	std::memset(input, 0, 10); //removing garbage values
	term.switchToNormalMode(); //set non-canonical params
	term.disableCursor();
	int search_mode = 0;
	page_Sptr page = pageMgr.getCurrPage(); //page_Sptr - Shared pointer of page class
	term.DrawView(page);
	CommandHandler cmdHandler; //command handler class to handle the different commands in command mode
	CommandParser cmdParser;   // command parser class to parse the commands by the user and convert as needed

	fflush(stdout);
	int read_byte_count = 0;
	while (read_byte_count = read(0, &input, 3))
	{
		if (term.mode == Mode::NORMAL)
		{
			if (search_mode == 0)
				page = pageMgr.getCurrPage();
			if (read_byte_count == 3 && input[0] == '\033')
			{
				//read(0, &input, 1);
				if (input[1] == '[')
				{
					//read(0, &input, 1);
					switch ((int)input[2])
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
			else if (read_byte_count == 1 && (int)input[0] == (int)Action::KEY_ENTER) //When opening file or directory
			{
				try
				{
					page_Sptr new_page = page->enterDir();
					if (new_page != NULL) //if opening a folder
					{
						int curr_index = pageMgr.getCurrStateIndex();
						while (pageMgr.pageHistory.size() > curr_index + 1)
							pageMgr.pop();
						pageMgr.push(new_page);
						page = pageMgr.getCurrPage();
						if (search_mode == 1)
						{
							search_mode = 0;
							term.search_flag = 0;
						}
						term.DrawView(page);
					}
				}
				catch (Error e)
				{
					term.DrawError(e.getErrorMsg());
				}
			}
			else if (read_byte_count == 1 && (int)input[0] == (int)Action::KEY_BACKSPACE) //to handle backspace when in normal mode
			{
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
			else if (read_byte_count == 1 && (int)input[0] == (int)Action::KEY_h) //jumping to home directory
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
			else if (read_byte_count == 1 && search_mode == 0 && (int)input[0] == (int)Action::KEY_COLON) //switching to command mode
			{
				term.enableCursor();
				term.DrawCommand("");
				char buffer[1024];
				std::memset(buffer, 0, 1024);
				char input[10];
				std::memset(input, 0, 10);
				int start = 0;
				while (true) //reading while in command mode
				{
					try
					{
						std::memset(input, 0, 10);
						int byte_count = read(0, input, 5);
						if (byte_count == 1 && input[0] != '\n' && ((int)input[0] != (int)Action::KEY_ESC)) //while user typing input, including backspace
						{
							if ((int)input[0] != (int)Action::KEY_BACKSPACE)
							{
								buffer[start] = input[0];
								start++;
							}
							else if (start > 0)
							{
								buffer[start - 1] = 0;
								start--;
							}
							term.DrawCommand(std::string(buffer));
						}
						else if (byte_count == 1 && input[0] == '\n') //when ENTER is pressed in command mode
						{
							std::vector<std::string> command_args = cmdParser.getArgs(std::string(buffer));
							if (command_args.size() <= 1)
							{
								throw Error("No Arguments");
							}
							std::vector<std::string> translated_args = cmdParser.translateArgs(command_args, page);
							if (translated_args[0] == "copy") //copy command
							{
								cmdHandler.copyFiles(translated_args);
								pageMgr.updateCurrPage();
								page = pageMgr.getCurrPage();
								term.DrawView(page);
							}
							else if (translated_args[0] == "move") //move command
							{
								cmdHandler.copyFiles(translated_args);
								cmdHandler.delFiles(translated_args);
								pageMgr.updateCurrPage();
								page = pageMgr.getCurrPage();
								term.DrawView(page);
							}
							else if (translated_args[0] == "rename") //rename command
							{
								if (command_args.size() != 3)
								{
									throw Error("Invalid Arguments");
								}
								cmdHandler.rename(translated_args[1], translated_args[2]);
								pageMgr.updateCurrPage();
								page = pageMgr.getCurrPage();
								term.DrawView(page);
							}
							else if (translated_args[0] == "create_file") //create_file command
							{
								if (command_args.size() != 3)
								{
									throw Error("Invalid Arguments");
								}
								cmdHandler.createFile(command_args[1], translated_args[2]);
								pageMgr.updateCurrPage();
								page = pageMgr.getCurrPage();
								term.DrawView(page);
							}
							else if (translated_args[0] == "create_dir") //create_dir command
							{
								if (command_args.size() != 3)
								{
									throw Error("Invalid Arguments");
								}
								cmdHandler.createDir(command_args[1], translated_args[2]);
								pageMgr.updateCurrPage();
								page = pageMgr.getCurrPage();
								term.DrawView(page);
							}
							else if (translated_args[0] == "delete_file") //delete_file command
							{
								if (command_args.size() != 2)
								{
									throw Error("Invalid Arguments");
								}
								cmdHandler.delFile(translated_args[1]);
								pageMgr.updateCurrPage();
								page = pageMgr.getCurrPage();
								term.DrawView(page);
							}
							else if (translated_args[0] == "delete_dir") //delete_dir command
							{
								if (command_args.size() != 2)
								{
									throw Error("Invalid Arguments");
								}
								cmdHandler.delDir(translated_args[1]);
								pageMgr.updateCurrPage();
								page = pageMgr.getCurrPage();
								term.DrawView(page);
							}
							else if (translated_args[0] == "goto") //goto command
							{
								if (command_args.size() != 2)
								{
									throw Error("Invalid Arguments");
								}
								auto new_page = cmdHandler.goToDir(translated_args[1]);
								int curr_index = pageMgr.getCurrStateIndex();
								while (pageMgr.pageHistory.size() > curr_index + 1)
									pageMgr.pop();
								pageMgr.push(new_page);
								page = pageMgr.getCurrPage();
								term.DrawView(page);
							}
							else if (translated_args[0] == "search") //search command
							{
								if (command_args.size() != 2)
								{
									throw Error("Invalid Arguments");
								}
								std::vector<std::string> search_output;
								cmdHandler.search(command_args[1], Path::getInstance().getSystemAbsPath(page->cwd), search_output);
								if (search_output.size() == 0)
								{
									throw Error("No search results found");
								}
								page = std::make_shared<Page>(Page(search_output));
								term.search_flag = 1;
								search_mode = 1;
								term.DrawView(page);
								break;
							}
							else if (translated_args[0] == "snapshot") //snapshot command
							{
								if (command_args.size() != 3)
								{
									throw Error("Invalid Arguments");
								}
								cmdHandler.snapshot(translated_args[1], translated_args[2]);
								pageMgr.updateCurrPage();
								page = pageMgr.getCurrPage();
								term.DrawView(page);
							}
							else
							{
								throw Error("Invalid Command"); //for incorrect commands
							}
							term.eraseStatusBar();
							term.DrawCommand("");
							std::memset(buffer, 0, 1024);
							start = 0;
						}
						else if (byte_count == 1 && (int)input[0] == (int)Action::KEY_ESC) //returning to normal mode
							break;
					}
					catch (Error e) //catching errors and printing accordingly
					{
						term.DrawError(e.getErrorMsg());
						term.eraseStatusBar();
						term.DrawCommand("");
						std::memset(buffer, 0, 1024);
						start = 0;
					}
				}
				term.DrawView(page);
				term.disableCursor();
			}
			else if (read_byte_count == 1 && search_mode == 0 && (int)input[0] == (int)Action::KEY_q) //quitting application in normal mode
			{
				term.switchToCommandMode();
				term.enableCursor();
				std::cout << "\e[?1049l" << std::flush; //restoring old screen before launching the application
				return (0);
			}
		}
		std::memset(input, 0, 10);
	}
	return (0);
}