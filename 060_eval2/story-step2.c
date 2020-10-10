#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

/* Cite function below: AoP page 269 */
size_t contains(catarray_t * cats, char * catName) {
  for (size_t i = 0; i < cats->n; i++) {
    if (!strcmp(cats->arr[i].name, catName)) {
      return i;  // cats contains this cat name, return name index
    }
  }
  return 0;  // cat does not contain this cat name
}

char * getWord(char * line) {
  char * newLine = strchr(line, '\n');
  *newLine = '\0';  // strip newline character
  char * wordPtr = strchr(line, ':');
  if (wordPtr == NULL) {
    fprintf(stderr, "No ':' in line below:\n%s\n", line);
    exit(EXIT_FAILURE);
  }
  wordPtr++;
  char * word = strdup(wordPtr);
  return word;
}

// getCat strip first ':' and characters after ':'
// So, it must be used after getting word with getWord
char * getCat(char * line) {
  char * ptr = strchr(line, ':');
  if (ptr == NULL) {
    fprintf(stderr, "No ':' in line below:\n %s\n", line);
    exit(EXIT_FAILURE);
  }
  *ptr = '\0';  //strip character ':' and characters after ':'
  char * cat = strdup(line);
  return cat;
}

catarray_t * parseCategories(FILE * f) {
  char * line = NULL;
  size_t linecap;
  catarray_t * cats = malloc(sizeof(*cats));
  cats->n = 0;
  cats->arr = NULL;
  while (getline(&line, &linecap, f) >= 0) {
    // get category name and words from line
    char * currWord = getWord(line);
    // save them by updating cats
    char * currCat = getCat(line);
    size_t currCatIdx = contains(cats, currCat);
    // printf("Parsed Cat %s: %s\n", currCat, currWord);
    if (currCatIdx == 0) {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = currCat;
      cats->n++;
      cats->arr[cats->n - 1].n_words = 1;
      cats->arr[cats->n - 1].words = malloc(1 * sizeof(*cats->arr[cats->n - 1].words));
      cats->arr[cats->n - 1].words[0] = currWord;
    }
    else {
      free(currCat);
      cats->arr[currCatIdx].words = realloc(
          cats->arr[currCatIdx].words,
          (cats->arr[currCatIdx].n_words + 1) * sizeof(*cats->arr[currCatIdx].words));
      cats->arr[currCatIdx].words[cats->arr[currCatIdx].n_words] = currWord;
      cats->arr[currCatIdx].n_words++;
    }
  }
  free(line);
  return cats;
}

void freeCatArr(catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    for (size_t j = 0; j < cats->arr[i].n_words; j++) {
      free(cats->arr[i].words[j]);
    }
    free(cats->arr[i].words);
    free(cats->arr[i].name);
  }
  free(cats->arr);
  free(cats);
}

// Takes one command line argument: name of file with the categories and words.
// Reads file and stores the words into a catarray_t and print them.
int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Useage: story_step2 wordFileName\n");
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
