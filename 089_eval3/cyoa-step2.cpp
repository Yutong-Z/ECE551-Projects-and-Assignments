#include "cyoa.h"

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
