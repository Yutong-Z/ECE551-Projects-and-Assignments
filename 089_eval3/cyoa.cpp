#include "cyoa.h"

/*
Step1
 */

void Page::printLines() {
  std::vector<std::string>::iterator it = lines->begin();
  while (it != lines->end()) {
    std::cout << *it << "\n";
    ++it;
  }
}

void midPage::printPage() {
  printLines();
  std::cout << "\n"
            << "What would you like to do?\n\n";
  std::vector<Choice>::iterator it = nav->begin();
  unsigned int i = 1;
  while (it != nav->end()) {
    std::cout << i;
    it->printChoice();
    ++it;
    i++;
  }
}
void midPage::parseNavLine(std::string & line) {
  size_t colonIdx = line.find(':');
  if (colonIdx == std::string::npos) {
    std::cerr << "No ':' found in this choice navigation line below:\n"
              << line << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string pageNumStr = line.substr(0, colonIdx);
  unsigned int pageNum = getPageNum(pageNumStr);
  if (pageNum == 0) {
    std::cerr << "Invaild page number " << pageNumStr << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string text = line.substr(colonIdx + 1);
  nav->push_back(Choice(pageNum, text));
}

void endPage::printPage() {
  printLines();
  std::cout << "\n";
  if (ifWin == 1) {
    std::cout << "Congratulations! You have won. Hooray!" << std::endl;
  }
  else {
    std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
  }
}

void endPage::parseNavLine(std::string & line) {
  if (line.compare("WIN") == 0) {
    ifWin = 1;
  }
  else if (line.compare("LOSE") == 0) {
    ifWin = 0;
  }
  else {
    std::cerr << "Input line of parseStatus is not WIN\n or LOSE\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

/*
Convert a string pageNumStr to an unsigned int page numer.
Input:
  std::string & pageNumStr: A reference to string parsed from the page file, navigation part, the substring before ':' in choice line.
Returns:
  0: If the input string represents negative number or contains not digital char.
  unsigned int pageNum: If the input string could be converted to vaild page number.
 */
unsigned int getPageNum(std::string & pageNumStr) {
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

Page * parsePage(std::ifstream & f, const unsigned int pageNum) {
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
    std::cerr << "Cannot find line begins with '#' after navigation part!" << std::endl;
    exit(EXIT_FAILURE);
  }
  while (getline(f, line)) {
    currPage->addLine(line);
  }
  return currPage;
}

/*
Step2
 */
