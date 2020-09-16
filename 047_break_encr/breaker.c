#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int FindMax(int * array, int size) {
  int max = 0;
  for (int i = 1; i < size; i++) {
    if (array[i] > array[max]) {
      max = i;
    }
  }
  return max;
}
// find the biggest value's index

int * CountChar(FILE * f, int * toCount) {
  int c;
  while ((c = fgetc(f) != EOF)) {
    if (isalpha(c)) {
      c = tolower(c);
      c -= 'a';
      if ((c >= 0) && (c < 26)) {
        toCount[c] += 1;
      }
      else {
        printf("Encountered can't recognized character!");
      }
    }
  }
  return toCount;
}
// count the occurance of all characters in file

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: break inputFileName\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file!");
    return EXIT_FAILURE;
  }

  int toCount[26] = {0};
  int * Counted = CountChar(f, toCount);
  int m = FindMax(Counted, 26);

  // 'e' is 'a'+4
  int key = (m - 4 + 26) % 26;
  fprintf(stdout, "The key is %d\n", key);

  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
