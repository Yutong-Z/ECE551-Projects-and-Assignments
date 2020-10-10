#include "rand_story.h"

/*
STEP1
*/

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
    fprintf(stderr, "Underscore not matcing in line: %s", line);
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

/*
STEP2
*/

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
