#include "rand_story.h"

/*
STEP1
*/

void printStoryLine(char * line, catarray_t * cats, int reuse, category_t * track) {
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
      const char * word =
          fancyChooseWord(cat, cats, track);  // step1, get a pointer to "cat"
      catLen = 0;
      j = 0;
      i++;
      // print word into story
      printf("%s", word);
      // track used words
      track->words = realloc(track->words, (track->n_words + 1) * sizeof(*track->words));
      track->words[track->n_words] = strdup(word);
      track->n_words++;
      // reuse = 0, remove words if it's not from track
      if (cat != NULL && reuse == 0) {
        if (atoi(cat) < 1 && contains(cats, cat) != -1 &&
            cats->arr[contains(cats, cat)].n_words !=
                0) {  // determine if word is not chose from track
          removeWord(cats, cat, word);
        }
      }
    }
  }
  free(cat);
  if (j == 1) {  //no matching '_' with a pervious '_'
    fprintf(stderr, "Underscore not matcing in line: %s", line);
    exit(EXIT_FAILURE);
  }
}

void freeCat(category_t * cat) {
  for (size_t i = 0; i < cat->n_words; i++) {
    free(cat->words[i]);
  }
  free(cat->words);
  free(cat->name);
  free(cat);
}

void parseTemplate(FILE * f, catarray_t * cats, int reuse) {
  char * line = NULL;
  size_t linecap;
  category_t * track = malloc(sizeof(*track));
  track->n_words = 0;
  track->words = NULL;
  track->name = strdup("used");
  // parsing temlapte file, print lines with blank converted
  while (getline(&line, &linecap, f) >= 0) {
    printStoryLine(line, cats, reuse, track);
  }
  free(line);
  freeCat(track);
}

/*
STEP2
*/

/* Cite function below: AoP page 269 */
int contains(catarray_t * cats, char * catName) {
  for (size_t i = 0; i < cats->n; i++) {
    if (!strcmp(cats->arr[i].name, catName)) {
      return i;  // cats contains this cat name, return name index
    }
  }
  return -1;  // cats does not contain this cat name
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
    int currCatIdx = contains(cats, currCat);
    // printf("Parsed Cat %s: %s\n", currCat, currWord);
    if (currCatIdx == -1) {  // new category
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = currCat;
      cats->n++;
      cats->arr[cats->n - 1].n_words = 1;
      cats->arr[cats->n - 1].words = malloc(1 * sizeof(*cats->arr[cats->n - 1].words));
      cats->arr[cats->n - 1].words[0] = currWord;
    }
    else {  // category exits in catarray "cats"
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

/*
STEP3
*/

/*
Fancy version of chooseWord() function that returns words from cats:
Returns:
  If second argument "cats" is NULL, returns a pointer to string "cat"
  If "cats" is not NULL: 
    1. If category name is a valid integer of at least one, call this integer num, returns num-th previously used word
    2. If the category name is a category in "cats", returns a random word with chooseWord()
    3. Otherwise, exit failure
Inputs:
  char * category: a string representing the category name
  catarray_t * cats: categories to choose word from
  category * track: struct contain sequence of previously used words
 */
const char * fancyChooseWord(char * category, catarray_t * cats, category_t * track) {
  if (cats == NULL) {
    return chooseWord(category, NULL);
  }
  else {
    int num = atoi(category);  // atoi returns 0 if category can't be converted
    if (num >= 1 && (unsigned int)num <= track->n_words) {
      // category name is a valid integer of at least one
      // and the integer less than number of previously used words
      return track->words[track->n_words - (unsigned int)num];
    }
    else if (num >= 1 && (unsigned int)num <= track->n_words) {
      // integer is larger than number of previously used words
      fprintf(stderr, "Integer category _%d_ larger than previously used words!", num);
      exit(EXIT_FAILURE);
    }
    else if (contains(cats, category) != -1 &&
             cats->arr[contains(cats, category)].n_words != 0) {
      // cats contains category && the category still has words (for option in step4)
      return chooseWord(category, cats);
    }
    else {
      fprintf(stderr, "catarray doesn't contain category %s\n", category);
      exit(EXIT_FAILURE);
    }
  }
}

/*
STEP4
*/

/*
Remove a word from categories struct "cats"
After removing the word, if the category it belongs to has no word, n_word is category will be 0
Not removing the category without any words to avoid double free happen
 */
void removeWord(catarray_t * cats, char * category, const char * word) {
  int catIdx = contains(cats, category);
  size_t wordIdx;
  for (size_t i = 0; i < cats->arr[catIdx].n_words; i++) {
    if (strcmp(word, cats->arr[catIdx].words[i]) == 0) {
      wordIdx = i;
    }
  }
  free(cats->arr[catIdx].words[wordIdx]);
  cats->arr[catIdx].n_words--;
  size_t j = 0;
  char ** temp = malloc(cats->arr[catIdx].n_words * sizeof(*temp));
  for (size_t i = 0; i < (cats->arr[catIdx].n_words + 1); i++) {
    if (i != wordIdx) {
      temp[j] = cats->arr[catIdx].words[i];
      j++;
    }
  }
  free(cats->arr[catIdx].words);
  cats->arr[catIdx].words =
      malloc(cats->arr[catIdx].n_words * sizeof(*cats->arr[catIdx].words));
  for (size_t i = 0; i < cats->arr[catIdx].n_words; i++) {
    cats->arr[catIdx].words[i] = temp[i];
  }
  free(temp);
}
