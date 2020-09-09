#include <stdio.h>
#include <stdlib.h>

size_t max(size_t x, size_t y) {
  if (x > y) {
    return x;
  }
  else {
    return y;
  }
}

size_t maxSeq(int * array, size_t n) {
  if (n == 0) {
    return 0;
  }

  size_t total_m = 1;
  size_t m = 1;
  for (size_t i = 0; i < (n - 1); i++) {
    if (array[i] < array[i + 1]) {
      m += 1;
    }
    else {
      m = 1;
    }
    total_m = max(total_m, m);
  }
  return total_m;
}
