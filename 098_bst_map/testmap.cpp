#include "bstmap.h"
#include "map.h"

int main() {
  BstMap<int, int> bm;
  bm.add(5, 5);
  bm.add(2, 1);
  bm.add(2, 2);
  bm.add(3, 3);
  bm.add(10, 10);

  bm.remove(2);
  bm.remove(3);

  bm.lookup(5);
  bm.lookup(10);
}
