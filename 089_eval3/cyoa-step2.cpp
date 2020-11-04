#include <set>

#include "cyoa.h"

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
      std::cout << "cant find file: " << fileName << std::endl;
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

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Useage: ./cyoa-step2 storyDirctoryName\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  // read each page, total vaild page number is size of pages vector
  std::vector<Page *> pages = readPages(argv[1]);
  // check reference of each vaild page
  checkReference(pages);
  // create story
  playCyoa(pages);
  // delete each page
  deletePages(pages);
  return EXIT_SUCCESS;
}
