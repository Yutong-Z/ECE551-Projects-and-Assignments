#include "cyoa.h"

/*
prev pages will only be pages in reachable set 
 */
void fillPrevs(std::vector<Page *> & pages, std::set<Page *> & reachPage) {
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

unsigned int findWinPage(std::set<Page *> & reachPages) {
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
    std::vector<Page *> & pages,
    unsigned int winPageNum) {
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

void printWinPath(std::vector<std::pair<unsigned int, unsigned int> > & winPath) {
  // reversely print  winPath to make it from first page to win page
  std::vector<std::pair<unsigned int, unsigned int> >::reverse_iterator itPair =
      winPath.rbegin();
  while (itPair + 1 != winPath.rend()) {
    std::cout << "Page " << (*itPair).first << " Choice " << (*itPair).second << "\n";
    ++itPair;
  }
  std::cout << "Page " << (*itPair).first << " WIN\n";
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Useage: ./cyoa-step4 storyDirctoryName\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  // read each page, total vaild page number is size of pages vector
  std::vector<Page *> pages = readPages(argv[1]);
  // check reference of each vaild page
  checkReference(pages);
  // get reachable set of pages
  std::set<Page *> reachPages = getReachableSet(pages);
  // calculate prev field of each page with prev refers to pages int set
  fillPrevs(pages, reachPages);
  // find a win page in reachable set
  unsigned int winPageNum = findWinPage(reachPages);
  if (winPageNum == 0) {  // exit if there is not a win page in set
    deletePages(pages);
    std::cout << "There is no way to win\n" << std::endl;
    return EXIT_SUCCESS;
  }
  // std::cout << "Win page is Page" << winPageNum << std::endl;
  // trace back and reverse to find path vertex
  std::vector<std::pair<unsigned int, unsigned int> > winPath =
      findWinPath(pages, winPageNum);
  // print path
  printWinPath(winPath);
  // delete each page
  deletePages(pages);
  return EXIT_SUCCESS;
}
