#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc < 2) {
    fprintf(stderr, "Useage: story_step1 storyFileName\n");
    exit(EXIT_FAILURE);
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Can not open file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  parseTemplate(f);
  if (fclose(f) != 0) {
    fprintf(stderr, "Can not close file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
}
