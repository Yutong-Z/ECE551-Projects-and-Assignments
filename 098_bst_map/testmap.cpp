#include <assert.h>

#include "bstmap.h"
#include "map.h"
int main() {
  BstMap<int, int> bm;
  bm.add(5, 5);
  bm.add(2, 1);
  bm.add(2, 2);
  bm.add(3, 3);
  bm.add(10, 10);
  bm.printTree();

  bm.remove(5);
  bm.printTree();

  bm.add(1, 1);
  bm.remove(3);
  bm.printTree();

  bm.lookup(2);
  bm.lookup(10);

  BstMap<int, int> bm2;
  bm2.add(1, 1);
  bm2 = bm;
  assert(bm2.lookup(10) == bm.lookup(10));
}
