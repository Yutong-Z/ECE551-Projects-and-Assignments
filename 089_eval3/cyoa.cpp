#include "cyoa.h"

/*
Class Methods
 */

void Page::printLines() {
  std::vector<std::string>::iterator it = lines.begin();
  while (it != lines.end()) {
    std::cout << *it << "\n";
    ++it;
  }
}

void midPage::printPage() {
  printLines();
  std::cout << "\n"
            << "What would you like to do?\n\n";
  std::vector<Choice>::iterator it = nav.begin();
  unsigned int i = 1;
  while (it != nav.end()) {
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
  // check choice with duplicated page number
  std::vector<unsigned int> choices = this->getNav();
  std::vector<unsigned int>::iterator itChoice = choices.begin();
  while (itChoice != choices.end()) {
    if (pageNum == *itChoice) {
      std::cerr << "Choice refers to duplicated page number!" << std::endl;
      exit(EXIT_FAILURE);
    }
    ++itChoice;
  }
  // get the choice text and push back to nav field
  std::string text = line.substr(colonIdx + 1);
  nav.push_back(Choice(pageNum, text));
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

std::vector<unsigned int> midPage::getNav() {
  std::vector<unsigned int> choices;
  std::vector<Choice>::iterator it = nav.begin();
  while (it != nav.end()) {
    choices.push_back(it->getChoicePage());
    ++it;
  }
  return choices;
}

std::vector<unsigned int> endPage::getNav() {
  std::vector<unsigned int> choices;
  if (ifWin) {
    choices.push_back(1);
  }
  else {
    choices.push_back(0);
  }
  return choices;
}

/*
Step1
 */

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

Page * parsePage(std::ifstream & f, unsigned int pageNum) {
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
std::string getFileName(const unsigned int pageNum, char * directory) {
  std::stringstream ss;
  ss << directory << "/page" << pageNum << ".txt";
  std::string fileName = ss.str();
  return fileName;
}

std::vector<Page *> readPages(char * directory) {
  std::vector<Page *> pages = std::vector<Page *>();
  unsigned int pageNum = 1;
  std::string fileName;
  while (1) {
    fileName = getFileName(pageNum, directory);
    std::ifstream f;
    f.open(fileName);
    if (!f.is_open()) {  // break loop if no next page
      //std::cout << "cant find file: " << fileName << std::endl;
      break;
    }
    pages.push_back(parsePage(f, pageNum));
    pageNum++;
  }
  return pages;
}

void deletePages(std::vector<Page *> & pages) {
  std::vector<Page *>::iterator it = pages.begin();
  while (it != pages.end()) {
    delete *it;
    ++it;
  }
}

/*
verify conditions 4a 4b 4c
 */
void checkReference(std::vector<Page *> & pages) {
  unsigned int totalNum = pages.size();
  std::vector<Page *>::iterator itPage = pages.begin();
  bool haveWin = 0;
  bool haveLose = 0;
  std::set<unsigned int> refPages;  // A set of page that have been referenced
  while (itPage != pages.end()) {
    if ((*itPage)->checkEnd()) {          // encounter end page
      if ((*itPage)->getNav()[0] == 0) {  // end page is lose
        haveLose = 1;
      }
      if ((*itPage)->getNav()[0] == 1) {  // end page is win
        haveWin = 1;
      }
    }
    else {  // encounter mid page(page with choices)
      std::vector<unsigned int> currChoices = (*itPage)->getNav();
      std::vector<unsigned int>::iterator itChoice = currChoices.begin();
      while (itChoice != currChoices.end()) {
        if (*itChoice > totalNum || *itChoice == 0) {
          std::cerr << "Choice refer to invalid page number: " << *itChoice << std::endl;
          exit(EXIT_FAILURE);
        }
        refPages.insert(*itChoice);  // add the page referenced to set
        ++itChoice;
      }
    }
    ++itPage;
  }
  // check set
  refPages.insert(1);  // page 1 is not asked to be referenced
  if (refPages.size() != totalNum) {
    std::cerr << "Not referenced valid page(s) exist!" << std::endl;
    exit(EXIT_FAILURE);
  }
  // check win/lose
  if (haveWin != 1 || haveLose != 1) {
    std::cerr << "Can find WIN page or can find LOSE page!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void playCyoa(std::vector<Page *> & pages) {
  Page * currPage = pages[0];  // page1 is first element in vector
  while (currPage->checkEnd() == 0) {
    // print current page
    currPage->printPage();
    // get an input string
    std::string input;
    std::cin >> input;
    unsigned int choiceNum = getPageNum(input);  // get the choice num form string
    while (choiceNum == 0 || choiceNum > currPage->getNav().size()) {
      // getPageNum() returns 0 if input string contains not digital char
      // get another input if previous input is not a vaild number or larger than choice amount
      std::cout << "That is not a valid choice, please try again\n";
      std::string input2;
      std::cin >> input2;
      choiceNum = getPageNum(input2);
    }
    // get choice number's corresponding page number
    unsigned int pageNum = currPage->getNav()[choiceNum - 1];
    // pages were pushed to back of vector form first to last in sequence
    currPage = pages[pageNum - 1];
  }
  // print the last page with WIN or LOSE
  currPage->printPage();
}

/*
Step3
*/
