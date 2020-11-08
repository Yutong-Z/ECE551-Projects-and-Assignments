#include "cyoa.h"

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
    std::cout << "There is no way to win" << std::endl;
    return EXIT_SUCCESS;
  }
  // std::cout << "Win page is Page" << winPageNum << std::endl;
  // trace back and reverse to find path vector
  std::vector<std::pair<unsigned int, unsigned int> > winPath =
      findWinPath(pages, winPageNum);
  // print path
  printWinPath(winPath);
  // delete each page
  deletePages(pages);
  return EXIT_SUCCESS;
}
