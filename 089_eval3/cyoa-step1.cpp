#include <cstdlib>
#include <fstream>
#include <sstream>

#include "cyoa.h"

/*
Have to use C++11
unsigned int getPageNum(char * fileName) {
  std::string name(fileName + 4);
  size_t idx = 0;
  unsigned int pageNum = std::stoul(name, &idx);  // throw exceptions when cannot convert
  if (name[idx] != '.') {
    printf("Invalid story page file name!\n");
    exit(EXIT_FAILURE);
  }
  return pageNum;
}
*/

Page * parsePage(std::ifstream & f, std::string & pageNum) {
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
    std::cerr << "Cannot find line begins with '#' !" << std::endl;
    exit(EXIT_FAILURE);
  }
  while (getline(f, line)) {
    currPage->addLine(line);
  }
  return currPage;
}

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
  std::stringstream ss;
  ss << 0;
  std::string currPageNum = ss.str();
  Page * currPage = parsePage(f, currPageNum);
  // print page
  currPage->printPage();
  // free page
  delete currPage;
  f.close();
  return EXIT_SUCCESS;
}
