#include "cyoa.h"

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
