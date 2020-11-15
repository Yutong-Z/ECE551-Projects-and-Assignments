#include "cyoa.h"

/*
Class Methods
 */

/*
Method that prints every story line in the lines field of a midPage or endPage instance
 */
void Page::printLines() const {
  std::vector<std::string>::const_iterator it = lines.begin();
  while (it != lines.end()) {
    std::cout << *it << "\n";
    ++it;
  }
}

/*
A virtual method of midPage (child class of Page)
that prints choices and story of the corresponding page to output
 */
void midPage::printPage() const {
  printLines();
  std::cout << "\n"
            << "What would you like to do?\n\n";
  std::vector<Choice>::const_iterator it = nav.begin();
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
void endPage::printPage() const {
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
std::vector<unsigned int> midPage::getNav() const {
  std::vector<unsigned int> choices;
  std::vector<Choice>::const_iterator it = nav.begin();
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
std::vector<unsigned int> endPage::getNav() const {
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
unsigned int getNumFromStr(const std::string & strNum) {
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
    currPage = new endPage(pageNum, 0);  // construct endPage
    currPage->parseNavLine(line);
    getline(f, line);  // get the line starts with #
  }
  // first line is choice line
  else {
    currPage = new midPage(pageNum);  // construc midPage
    currPage->parseNavLine(line);
    while (getline(f, line)) {
      if (line[0] == '#') {
        break;  // Do nothing with the first line starts with '#'
      }
      currPage->parseNavLine(line);
    }
  }
  // now, line shold be the line starts with '#'
  if (line[0] != '#') {
    std::cerr << "Cannot find line begins with '#' after navigation part!" << std::endl;
    exit(EXIT_FAILURE);
  }
  // add all lines after the '#' line into Page instance
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
std::string getFileName(const unsigned int pageNum, const char * directory) {
  std::stringstream ss;
  ss << directory << "/page" << pageNum << ".txt";
  std::string fileName = ss.str();
  return fileName;
}

/*
Function that read all pages with consecutive page number starting from 1 in a directory
Input:
  directory: an array of characters that represents the directory
Return:
  A vector of pointer to Page (minPage or endPage) that are parsed with sequence of page number of each page 
 */
std::vector<Page *> readPages(const char * directory) {
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

/*
Function that delet every Page that the input vector points to
Input:
  pages: A reference to a vector of pointer to Page that are allocted on heap
 */
void deletePages(std::vector<Page *> & pages) {
  std::vector<Page *>::iterator it = pages.begin();
  while (it != pages.end()) {
    delete *it;
    ++it;
  }
}

/*
Function that verify conditions 4a 4b 4c in step2 for the group of pages pointed by input vector
4a. Every page that is referenced by a choice is valid.
4b. Every page (except page 1) is referenced by at least one *other* page's choices.
4c. At least one page must be a WIN page and at least one page must be a LOSE page.
Input:
  pages: A reference to a vector of pointer to Page that are allocted on heap
 */
void checkReference(const std::vector<Page *> & pages) {
  unsigned int totalNum = pages.size();
  std::vector<Page *>::const_iterator itPage = pages.begin();
  bool haveWin = 0;
  bool haveLose = 0;
  std::set<unsigned int> refPages;  // A set of page that have been referenced
  while (itPage != pages.end()) {
    if ((*itPage)->checkEnd()) {          // encounter end page (page with WIN or LOSE)
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
    std::cerr << "Cannot find WIN page or cannot find LOSE page!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

/*
Function that plays the choose your own adventure game with user
and prints pages with users' choices
with the beginning page, page 1, always printed
Input:
  pages: A reference to a vector of pointer to Page that are allocted on heap
 */
void playCyoa(const std::vector<Page *> & pages) {
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

/*
Function that checks for page reachablilty
and insert the pointers points to each reachable page into a set
Input:
  pages: A reference to a vector of pointer to Page that are allocted on heap
Return:
  A set that contains pointers to all reachable pages
 */
std::set<Page *> getReachableSet(const std::vector<Page *> & pages) {
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

/*
Fnuction that prints out the message of the unreachable pages of story.
Inputs:
  reachPages: A set that contains pointers to all reachable pages
  totalPageNum: The number of pages in the story (pages with not consecutive page number doesn't count)
 */
void printUnreach(const std::set<Page *> & reachPages, const unsigned int totalPageNum) {
  // for each page's number (num) in story
  for (unsigned int num = 1; num <= totalPageNum; num++) {
    bool found = 0;
    std::set<Page *>::iterator itPage = reachPages.begin();
    while (itPage != reachPages.end()) {
      // if there is a pointer to page in reachable set has the same page number with num
      // found num in reachable set, break the inner loop
      if ((*itPage)->currPageNum == num) {
        found = 1;
        break;
      }
      ++itPage;
    }
    // if cannot find num after check each page's number in reachable set
    // print the message of the unreachable page number (num)
    if (found == 0) {
      std::cout << "Page " << num << " is not reachable\n";
    }
  }
}

/*
Step4
 */

/*
Function that fills the prev field of all pages in story.
prev pages number and choice pair filled in will only be pages and choices from reachable set 
Inputs:
  pages: A reference to a vector of pointer to Page that are allocted on heap
  reachPage: A set that contains pointers to all reachable pages 
 */
void fillPrevs(const std::vector<Page *> & pages, const std::set<Page *> & reachPages) {
  std::set<Page *>::iterator itPage = reachPages.begin();
  while (itPage != reachPages.end()) {
    // iterate through each page in reachable set
    if ((*itPage)->checkEnd() == 0) {
      // get choices of page if not WIN or LOSE page
      std::vector<unsigned int> choices = (*itPage)->getNav();
      // iterate through choices of page with for loop
      for (unsigned int choiceNum = 1; choiceNum <= choices.size(); choiceNum++) {
        // pageNum the choice refers to
        unsigned int pageNum = choices[choiceNum - 1];
        // push the pair to prevPages field of page that the choice refers to
        // prevPages.first is current page number iterated in set
        // prevPages.second is the iterated choice number in the current page intrated in set
        pages[pageNum - 1]->prevPages.push_back(
            std::pair<unsigned int, unsigned int>((*itPage)->currPageNum, choiceNum));
      }
    }
    ++itPage;
  }
}

/*
Function that returns a page's number with WIN navgation part in corresponding page.
Input:
  reachPages: A set that contains pointers to all reachable pages.
Return:
  A pages number with WIN navgation part in corresponding page.
  If there is miltiple WIN pages,
  the function returns the first WIN page it encountered from iteration on reachable page set. 
 */
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

/*
Function that traces back with prevous page information (prev filed)
from a win page to the start page (page 1)
and returns the trace back path, which is the reversed win path.
Inputs:
  pages:
  winPageNum:
Return:
  A vector of std::pair<unsigned int, unsigned int>
  For each pair, first filed is the page number
  second filed is the choice number to go to next page in win path.
  Because the vector returns is reversed win path,
  the first pair in the vector contains WIN page number,
  the last pair in the vector contains page number 1 and choice numer from page 1 to next page. 
 */
std::vector<std::pair<unsigned int, unsigned int> > findWinPath(
    const std::vector<Page *> & pages,
    const unsigned int winPageNum) {
  std::vector<std::pair<unsigned int, unsigned int> > winPath;
  unsigned int currPageNum = winPageNum;
  std::pair<unsigned int, unsigned int> currPair(winPageNum, 0);
  winPath.push_back(currPair);
  // while the first page has not been reached in trace back procedure
  while (currPageNum != 1) {
    // replace current page and choice pair to one of the prevous page and choice pair
    currPair = pages[currPageNum - 1]->prevPages.front();
    // push the new pair into path
    winPath.push_back(currPair);
    currPageNum = currPair.first;
  }
  return winPath;
}

/*
Function that print out the message of win path.
Input path is the reversed win path returned from finWinPth function,
so this function do iteration and print each pair reversely.
Input:
  winPath: A vector of std::pair<unsigned int, unsigned int> that should be returned from findWinPath function.
 */
void printWinPath(const std::vector<std::pair<unsigned int, unsigned int> > & winPath) {
  // reversely print reversed win path to make it from first page to win page
  std::vector<std::pair<unsigned int, unsigned int> >::const_reverse_iterator itPair =
      winPath.rbegin();
  while (itPair + 1 != winPath.rend()) {
    std::cout << "Page " << (*itPair).first << " Choice " << (*itPair).second << "\n";
    ++itPair;
  }
  std::cout << "Page " << (*itPair).first << " WIN\n";
}
