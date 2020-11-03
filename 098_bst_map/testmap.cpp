#include "bstmap.h"
#include "map.h"

int main() {
  BstMap<int, int> bm;
  bm.add(1, 1);
  bm.add(2, 1);
  bm.add(2, 2);

  bm.remove(2);
  //bm.remove(3);

  bm.lookup(1);
  //bm.lookup(2);
}
