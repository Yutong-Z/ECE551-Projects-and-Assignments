#include <cstdlib>
#include <fstream>
#include <sstream>

#include "cyoa.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Useage: ./cyoa-step1 pageFileName" << std::endl;
    return EXIT_FAILURE;
  }
  std::ifstream f;
  f.open(argv[1]);
  if (!f.is_open()) {
    std::cerr << "Can not open file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }
  // parse page
  Page * currPage = parsePage(f, 0);
  // print page
  currPage->printPage();
  // free page
  delete currPage;
  f.close();
  return EXIT_SUCCESS;
}
