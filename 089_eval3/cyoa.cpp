#include "cyoa.h"

/*
Step1
 */

/*
Convert a string pageNumStr to an unsigned int page numer.
Input:
  std::string pageNumStr: A string parsed from the page file, navigation part, the substring before ':' in choice line.
Returns:
  0: If the input string represents negative number or contains not digital char.
  unsigned int pageNum: If the input string could be converted to vaild page number.
 */
unsigned int getPageNum(std::string pageNumStr) {
  unsigned int pageNum;
  std::stringstream ss1(pageNumStr);
  ss1 >> pageNum;
  std::stringstream ss2;
  ss2 << pageNum;  // Convert the extracted number back to stringstream
  if (pageNumStr.compare(ss2.str()) != 0) {
    // Invalid page number pageNumStr if the string converted back is different from input
    return 0;
  }
  else {
    return pageNum;
  }
}

Page * parsePage(std::ifstream & f, std::string & pageNum) {
  Page * currPage;
  std::string line;
  if (!getline(f, line)) {  // check if first line empty
    std::cerr << "Empty Page!" << std::endl;
    exit(EXIT_FAILURE);
  }
  // first line is WIN or LOSE status line
  if (line.compare("WIN") == 0 || line.compare("LOSE") == 0) {
    currPage = new endPage(pageNum, 0);
    currPage->parseNavLine(line);
    getline(f, line);  // get the line starts with #
  }
  // first line is choice line
  else {
    currPage = new midPage(pageNum);
    currPage->parseNavLine(line);
    while (getline(f, line)) {
      if (line[0] == '#') {
        break;  // Do nothing with the first line starts with '#'
      }
      currPage->parseNavLine(line);
    }
  }
  if (line[0] != '#') {
    std::cerr << "Cannot find line begins with '#' !" << std::endl;
    exit(EXIT_FAILURE);
  }
  while (getline(f, line)) {
    currPage->addLine(line);
  }
  return currPage;
}
