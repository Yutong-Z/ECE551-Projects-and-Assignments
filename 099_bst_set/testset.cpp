#include <cstdlib>

#include "bstset.h"
#include "set.h"
int main(void) {
  BstSet<int> bs;
  bs.add(5);
  bs.add(1);
  bs.add(10);
  bs.add(9);
  bs.add(11);
  bs.printTree();

  bs.remove(5);
  bs.remove(10);
  bs.printTree();

  return EXIT_SUCCESS;
}
