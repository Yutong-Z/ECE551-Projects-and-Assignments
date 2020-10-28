#include <stdio.h>

#include "function.h"
int binarySearchForZero(Function<int, int> * f, int low, int high) {
  if ((high - low) <= 1) {
    return low;
  }
  int mid = (low + high) / 2;
  int fx = f->invoke(mid);
  if (fx == 0) {
    return mid;
  }
  else if (fx < 0) {
    return binarySearchForZero(f, mid, high);
  }
  else {  // fx>0
    return binarySearchForZero(f, low, mid);
  }
}
