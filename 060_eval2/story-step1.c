#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"
#include "rand_story.h"

void printStoryLine(char * line) {
  size_t i = 0;
  int j = 0;
  size_t catLen = 0;
  char * cat = NULL;
  while (line[i] != '\0') {
    if (line[i] != '_' && j == 0) {  // out of blank
      printf("%c", line[i]);
      i++;
    }
    if (line[i] == '_' && j == 0) {  // new blank area
      i++;
      j = 1;
    }
    if (line[i] != '_' && j == 1) {  //in blank
      cat = realloc(cat, (catLen + 1) * sizeof(*cat));
      cat[catLen] = line[i];
      catLen++;
      i++;
    }
    if (line[i] == '_' && j == 1) {  //end of blank
      cat = realloc(cat, (catLen + 1) * sizeof(*cat));
      cat[catLen] = '\0';
      const char * word = chooseWord(cat, NULL);  // step1, get a pointer to "cat"
      catLen = 0;
      j = 0;
      i++;
      // print word into story
      printf("%s", word);
    }
  }
  free(cat);
  if (j == 1) {  //no matching '_' with a pervious '_'
    fprintf(stderr, "Underscore not matcing in line %s", line);
    exit(EXIT_FAILURE);
  }
}

void parseTemplate(FILE * f) {
  char * line = NULL;
  size_t linecap;
  // parsing temlapte file, print lines with blank converted
  while (getline(&line, &linecap, f) >= 0) {
    printStoryLine(line);
  }
  free(line);
}

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
