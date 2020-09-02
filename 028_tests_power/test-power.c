#include <stdio.h>
#include <stdlib.h>
unsigned power(unsigned x, unsigned y);

unsigned GetExpected(unsigned x, unsigned y) {
  unsigned p = 1;
  for (unsigned i = 0; i < y; i++) {
    p = p * x;
  }
  return p;
}

void run_check(unsigned x, unsigned y, unsigned expected_ans) {
  if (power(x, y) != expected_ans) {
    printf("I found broken program with input (%d, %d)!", x, y);
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  run_check(0, 0, GetExpected(0, 0));
  run_check(2, 0, GetExpected(2, 0));
  run_check(0, 2, GetExpected(0, 2));
  run_check(65535, 10, GetExpected(65535, 10));
  return EXIT_SUCCESS;
}
