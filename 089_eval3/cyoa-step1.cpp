#include <cstdlib>

#include "cyoa.h"
int main(void) {
  page * page1 = new endPage(1, 0);
  page1->lines->push_back("123\n");
  page1->lines->push_back("456\n");
  page1->printLines();
  delete page1;
  return EXIT_SUCCESS;
}
