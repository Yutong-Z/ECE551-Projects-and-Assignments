#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

// program takes two command line arguments:
//   1. the name of the file with the categories/words
//   2. the name of the file for the story template
int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Useage: story_step3 wordFileName templateFileName\n");
    exit(EXIT_FAILURE);
  }
  FILE * f1 = fopen(argv[1], "r");
  if (f1 == NULL) {
    fprintf(stderr, "Can not open word file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  // parse categories lines and store them in cats
  catarray_t * cats = parseCategories(f1);
  if (fclose(f1) != 0) {
    fprintf(stderr, "Can not close word file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  FILE * f2 = fopen(argv[2], "r");
  if (f2 == NULL) {
    fprintf(stderr, "Can not open template file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  // parse template ans print story with proper words
  parseTemplate(f2, cats);
  if (fclose(f2) != 0) {
    fprintf(stderr, "Can not close template file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  freeCatArr(cats);
}
