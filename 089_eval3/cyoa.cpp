#include "cyoa.h"

/*
Class Methods
 */

/*
Method that prints every story line in the lines field of a midPage or endPage instance
 */
void Page::printLines() {
  std::vector<std::string>::iterator it = lines.begin();
  while (it != lines.end()) {
    std::cout << *it << "\n";
    ++it;
  }
}

/*
A virtual method of midPage (child class of Page)
that prints choices and story of the corresponding page to output
 */
void midPage::printPage() {
  printLines();
  std::cout << "\n"
            << "What would you like to do?\n\n";
  std::vector<Choice>::iterator it = nav.begin();
  unsigned int i = 1;
  while (it != nav.end()) {
    std::cout << i << ". ";
    it->printChoice();
    ++it;
    i++;
  }
}

/*
A virtual method of midPage (child class of Page)
that parses a line from the navgation part (before "#" line) of a page file with choices
and adds the choice into the choices field of the midPage instance
Input:
  line: reference to std::string with choice content
 */
void midPage::parseNavLine(std::string & line) {
  size_t colonIdx = line.find(':');
  if (colonIdx == std::string::npos) {
    std::cerr << "No ':' found in this choice navigation line below:\n"
              << line << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string pageNumStr = line.substr(0, colonIdx);
  unsigned int pageNum = getNumFromStr(pageNumStr);
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

/*
A virtual method of endPage (child class of Page)
that prints ending and story of the corresponding page to output
 */
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

/*
A virtual method of endPage (child class of Page)
that parses a line from the navgation part (before "#" line) of a page file with WIN or LOSE line
and sets the ifWin field of the midPage instance as:
1 if line refers to  WIN or 0 if line refers to LOSE
Input:
  line: reference to std::string that should contain either "WIN" or "LOSE"
 */
void endPage::parseNavLine(std::string & line) {
  if (line.compare("WIN") == 0) {
    ifWin = 1;
  }
  else if (line.compare("LOSE") == 0) {
    ifWin = 0;
  }
  else {
    std::cerr << "Input line of parseStatus is neither WIN\n nor LOSE\n" << std::endl;
    exit(EXIT_FAILURE);
  }
}

/*
A virtual method of midPage (child class of Page)
that returns the page numbers in choices field as a vector
Returns:
  A vector that contains the page numbers in choices field
  with the sequence as they appear in navigation part of page file
*/
std::vector<unsigned int> midPage::getNav() {
  std::vector<unsigned int> choices;
  std::vector<Choice>::iterator it = nav.begin();
  while (it != nav.end()) {
    choices.push_back(it->getChoicePage());
    ++it;
  }
  return choices;
}

/*
A virtual method of endPage (child class of Page)
that returns a vector that contains the ending status in ifWin field of the page
Returns:
  Size 1 vector contains 1: if ifWin of this page is TRUE (win end page)
  Size 1 vector contains 0: if ifWin of this page is FALSE (lose end page)
*/
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
Function that converts a string strNum to an number that is larger than 0.
Input:
  strNum: A reference to string that should be conposed with digital characters and represents a positive number.
  In step1, the string is form the page file, navigation part, the substring before ':' in choice line.
Returns:
  0: If the input string represents negative number, zero, or contains not digital char.
  unsigned int pageNum: If the input string could be converted to vaild page number.
 */
unsigned int getNumFromStr(std::string & strNum) {
  unsigned int num;
  std::stringstream ss1(strNum);
  ss1 >> num;
  std::stringstream ss2;
  ss2 << num;  // Convert the extracted number back to stringstream
  if (strNum.compare(ss2.str()) != 0) {
    // Invalid page number pageNumStr if the string converted back is different from input
    return 0;
  }
  else {
    return num;
  }
}

/*
Function that parse one page file
Input:
  f: A reference to opened file in std::ifstream type
  pageNum: The page number of the page to be parsed (Ex. 11 for page11.txt)
Returns:
  A pointer to a instance of Page (static type) on heap
  If the page parsed is WIN or LOSE page, the dynamic type of instance returned is endPage
  If the page parsed contains choices, the dynamic type of instance returned is midPage
 */
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

/*
Function that converts an unsiged int to a string of file name starting with its directory
Inputs:
  pageNum: an unsigned int represents the page number of story book
  directory: an array of characters that represents the directory
Return:
  str::string contains file name starting with its directory
  (Ex. getFilename(11, "story") returns std::string that contains story/page11.txt)
 */
std::string getFileName(const unsigned int pageNum, char * directory) {
  std::stringstream ss;
  ss << directory << "/page" << pageNum << ".txt";
  std::string fileName = ss.str();
  return fileName;
}

/*
Function that pares all pages with consecutive page number starting from 1 in a directory
Input:
  directory: an array of characters that represents the directory
Return:
  A vector of pointer to pages that are parsed with sequence of page number of each page 
 */
std::vector<Page *> readPages(char * directory) {
  std::vector<Page *> pages = std::vector<Page *>();
  unsigned int pageNum = 1;
  std::string fileName;
  while (1) {
    fileName = getFileName(pageNum, directory);
    std::ifstream f;
    f.open(fileName);
    if (!f.is_open()) {  // break loop if next page does not exit
      //std::cout << "cant find file: " << fileName << std::endl;
      break;
    }
    pages.push_back(parsePage(f, pageNum));
    pageNum++;
    f.close();
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
    unsigned int choiceNum = getNumFromStr(input);  // get the choice num form string
    while (choiceNum == 0 || choiceNum > currPage->getNav().size()) {
      // getNumFromStr() returns 0 if input string contains not digital char, or represents negative number or zero
      // get another input if previous input is not a vaild number or larger than choice amount
      std::cout << "That is not a valid choice, please try again\n";
      std::string input2;
      std::cin >> input2;
      choiceNum = getNumFromStr(input2);
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
std::set<Page *> getReachableSet(std::vector<Page *> & pages) {
  std::set<Page *> reachPages;
  std::set<Page *> temp;
  reachPages.insert(pages[0]);  // insert pointer to page1
  while (temp.size() != reachPages.size()) {
    // while orignal set (temp) has equal size with changed set
    temp = reachPages;
    std::set<Page *>::iterator itTemp = temp.begin();
    while (itTemp != temp.end()) {
      // for each pointer to page in orignal set
      if ((*itTemp)->checkEnd() == 0) {
        // if page pointed is not WIN or LOSE page
        std::vector<unsigned int> choices = (*itTemp)->getNav();
        std::vector<unsigned int>::iterator itChoices = choices.begin();
        while (itChoices != choices.end()) {
          // for each choice(page number) in a page pointed
          reachPages.insert(pages[*itChoices - 1]);
          ++itChoices;
        }
      }
      ++itTemp;
    }
  }
  return reachPages;
}

void printUnreach(std::set<Page *> & reachPages, unsigned int totalPageNum) {
  for (unsigned int num = 1; num <= totalPageNum; num++) {
    bool found = 0;
    std::set<Page *>::iterator itPage = reachPages.begin();
    while (itPage != reachPages.end()) {
      if ((*itPage)->currPageNum == num) {
        found = 1;
        break;
      }
      ++itPage;
    }
    if (found == 0) {
      std::cout << "Page " << num << " is not reachable\n";
    }
  }
}

/*
Step4
 */

/*
prev pages will only be pages in reachable set 
 */
void fillPrevs(const std::vector<Page *> & pages, const std::set<Page *> & reachPage) {
  std::set<Page *>::iterator itPage = reachPage.begin();
  while (itPage != reachPage.end()) {
    // iterate through each page in reachable set
    if ((*itPage)->checkEnd() == 0) {
      // iterate through choices of page if not WIN or LOSE page
      std::vector<unsigned int> choices = (*itPage)->getNav();
      for (unsigned int choiceNum = 1; choiceNum <= choices.size(); choiceNum++) {
        // pageNum the choice refers to
        unsigned int pageNum = choices[choiceNum - 1];
        // push the pair to prevPages field of page that the choice refers to
        // prevPages.first is page number iterated in set
        // prevPages.second is the iterated choice number in the iterated page
        pages[pageNum - 1]->prevPages.push_back(
            std::pair<unsigned int, unsigned int>((*itPage)->currPageNum, choiceNum));
      }
    }
    ++itPage;
  }
}

unsigned int findWinPage(const std::set<Page *> & reachPages) {
  std::set<Page *>::iterator itPage = reachPages.begin();
  while (itPage != reachPages.end()) {
    if ((*itPage)->checkEnd() == 1 && (*itPage)->getNav()[0] == 1) {
      return (*itPage)->currPageNum;
    }
    ++itPage;
  }
  return 0;
}

std::vector<std::pair<unsigned int, unsigned int> > findWinPath(
    const std::vector<Page *> & pages,
    const unsigned int winPageNum) {
  std::vector<std::pair<unsigned int, unsigned int> > winPath;
  unsigned int currPageNum = winPageNum;
  std::pair<unsigned int, unsigned int> currPair(winPageNum, 0);
  winPath.push_back(currPair);
  while (currPageNum != 1) {
    currPair = pages[currPageNum - 1]->prevPages.front();
    winPath.push_back(currPair);
    currPageNum = currPair.first;
  }
  return winPath;
}

void printWinPath(const std::vector<std::pair<unsigned int, unsigned int> > & winPath) {
  // reversely print  winPath to make it from first page to win page
  std::vector<std::pair<unsigned int, unsigned int> >::const_reverse_iterator itPair =
      winPath.rbegin();
  while (itPair + 1 != winPath.rend()) {
    std::cout << "Page " << (*itPair).first << " Choice " << (*itPair).second << "\n";
    ++itPair;
  }
  std::cout << "Page " << (*itPair).first << " WIN\n";
}
