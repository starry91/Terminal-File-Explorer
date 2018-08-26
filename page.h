#ifndef PAGE_H
#define PAGE_H

#include <vector>
#include <string>
#include "file.h"
#include <memory>

class Page
{
public:
  std::vector<file_Sptr> files;
  int highlight_index;
  std::string cwd;

  Page(std::string);
  void scrollDown();
  void scrollUp();
  void enterDir();
};

typedef std::shared_ptr<Page> page_Sptr;

#endif