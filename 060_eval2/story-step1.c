#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"

// paragraph struct
struct para_tag {
  char ** lines;
  size_t lineNum;
};
typedef struct para_tag para_t;

void freePara(para_t * Para) {
  for (size_t i = 0; i < Para->lineNum; i++) {
    free(Para->lines[i]);
  }
  free(Para->lines);
  free(Para);
}

void printPara(para_t * Para) {
  for (size_t i = 0; i < Para->lineNum; i++) {
    printf("%s", Para->lines[i]);
  }
}

char * getStoryLine(char * line) {
  size_t i = 0;
  size_t storyLen = 0;
  char * story = NULL;
  int j = 0;
  size_t catLen = 0;
  char * cat = NULL;
  while (line[i] != '\0') {
    if (line[i] != '_' && j == 0) {  // out of blank
      story = realloc(story, (storyLen + 1) * sizeof(*story));
      story[storyLen] = line[i];
      storyLen++;
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
      // copy word into story
      // story = realloc(story, (storyLen + strlen(word)) * sizeof(*story));
      // story = strncpy(story + storyLen, word, strlen(word));
      // storyLen = storyLen + strlen(word);
      printf("%s: %s\n", cat, word);
      story = realloc(story, (storyLen + 1) * sizeof(*story));
      story[storyLen] = '?';
      storyLen = storyLen + 1;
    }
  }
  free(cat);
  story = realloc(story, (storyLen + 1) * sizeof(*story));
  story[storyLen] = '\0';
  if (j == 1) {  //no matching '_' with a pervious '_'
    fprintf(stderr, "Underscore not matcing in line %s", line);
    exit(EXIT_FAILURE);
  }
  return story;
}

para_t * parseTemplate(FILE * f) {
  char * curr = NULL;
  size_t linecap;
  para_t * template = malloc(sizeof(*template));
  template->lineNum = 0;
  template->lines = NULL;
  // parsing temlapte file, save lines into template struct
  while (getline(&curr, &linecap, f) >= 0) {
    template->lines =
        realloc(template->lines, (template->lineNum + 1) * sizeof(*template->lines));
    template->lines[template->lineNum] = curr;
    curr = NULL;
    template->lineNum++;
  }
  free(curr);
  // calculate story struct with tamplate sstruct
  para_t * story = malloc(sizeof(*story));
  story->lineNum = template->lineNum;
  story->lines = malloc(story->lineNum * sizeof(*story->lines));
  for (size_t i = 0; i < template->lineNum; i++) {
    // convert each original line into new allocated story line
    story->lines[i] = getStoryLine(template->lines[i]);
  }
  freePara(template);
  return story;
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
  // parse template lines and convert to story lines
  para_t * story = parseTemplate(f);
  // print story lines
  printPara(story);
  // free story lines memory
  freePara(story);
  if (fclose(f) != 0) {
    fprintf(stderr, "Can not close file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
}
