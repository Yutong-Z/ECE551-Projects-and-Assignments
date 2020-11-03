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
  std::vector<Page *>::iterator it = pages.begin();
  bool haveWin = 0;
  bool haveLose = 0;
  std::set<unsigned int> refPages;  // A set of page that have been referenced
  while (it != pages.end()) {
    if ((*it)->checkEnd()) {          // encounter end page
      if ((*it)->getNav()[0] == 0) {  // end page is lose
        haveLose = 1;
      }
      if ((*it)->getNav()[0] == 1) {
        haveWin = 1;
      }
    }
    else {  // encounter mid page(page with choices)
      std::vector<unsigned int> currChoices = (*it)->getNav();
      std::vector<unsigned int>::iterator itChoice = currChoices.begin();
      while (itChoice != currChoices.end()) {
        if (*itChoice > totalNum || *it == 0) {
          std::cerr << "choice refer to invalid page number: " << *it << std::endl;
          exit(EXIT_FAILURE);
        }
        refPages.insert(*itChoice);  // add the page referenced to set
      }
    }
    // check set
    // check win/lose
    ++it;
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Useage: ./cyoa-step2 storyDirctoryName\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  // read each page, count total page number
  std::vector<Page *> pages(readPages(argv[1]));
  pages[5]->printPage();
  // delete each page
  deletePages(pages);
  return EXIT_SUCCESS;
}
