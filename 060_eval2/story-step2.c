#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

// Takes one command line argument: name of file with the categories and words.
// Reads file and stores the words into a catarray_t and print them.
int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Useage: ./story_step2 wordFileName\n");
    exit(EXIT_FAILURE);
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Can not open file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  // parse categories lines and store them in cats
  catarray_t * cats = parseCategories(f);
  printWords(cats);
  freeCatArr(cats);
  if (fclose(f) != 0) {
    fprintf(stderr, "Can not close file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
}
