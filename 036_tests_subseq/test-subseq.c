#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n);

void run_check(int * array, size_t n, size_t expected_ans) {
  if (maxSeq(array, n) != expected_ans) {
    printf("I find broken program with an size [%u] array! \n", (unsigned int)n);
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  int array1[1] = {0};                             //1
  int array2[8] = {0, -3, -1, -2, -3, -2, -1, 0};  //4
  int array3[5] = {1, -2, 3, -4, 5};               //2
  int array4[7] = {1, 1, 1, 1, 2, 2, 2};           //2
  int array5[2] = {-37268, 37267};                 //2

  run_check(NULL, 0, 0);
  run_check(array1, 1, 1);
  run_check(array2, 8, 4);
  run_check(array3, 5, 2);
  run_check(array4, 7, 2);
  run_check(array5, 2, 2);

  return EXIT_SUCCESS;
}
