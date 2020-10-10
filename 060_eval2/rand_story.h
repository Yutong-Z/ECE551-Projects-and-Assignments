#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"

void printStoryLine(char * line, catarray_t * cats);
void parseTemplate(FILE * f, catarray_t * cats);

size_t contains(catarray_t * cats, char * catName);
char * getWord(char * line);
char * getCat(char * line);
catarray_t * parseCategories(FILE * f);
void freeCatArr(catarray_t * cats);

#endif
