#include "cyoa.h"

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

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Useage: ./cyoa-step3 storyDirctoryName\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  // read each page, total vaild page number is size of pages vector
  std::vector<Page *> pages = readPages(argv[1]);
  // check reference of each vaild page
  checkReference(pages);
  // check reachable of each page
  std::set<Page *> reachPages = getReachableSet(pages);
  // std::cout << "got reachable set" << std::endl;
  // print unreachable pages
  printUnreach(reachPages, pages.size());
  // delete each page
  deletePages(pages);
  return EXIT_SUCCESS;
}
