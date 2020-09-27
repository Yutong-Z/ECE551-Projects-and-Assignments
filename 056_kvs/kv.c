#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function that reads a line with format "kye=value\n"
// returns a poniter to typy kvpair_t with key and value in it
kvpair_t parseKVline(char * line) {
  kvpair_t pair;
  pair.key = NULL;
  pair.value = NULL;
  size_t i = 0;
  size_t j = 0;
  while (line[i] != '=' && line[i] != '\n' && line[i] != '\0') {
    pair.key = realloc(pair.key, (j + 1) * (*pair.key));
    pair.key[j] = line[i];
    i++;
    j++;
  }
  pair.key = realloc(pair.key, (j + 1) * (*pair.key));
  pair.key[j] = '\0';
  i++;  // form line[i]='=' to value area in line
  j = 0;
  while (line[i] != '\n' && line[i] != '\0') {
    pair.value = realloc(pair.value, (j + 1) * (*pair.value));
    pair.value[j] = line[i];
    i++;
    j++;
  }
  pair.value = realloc(pair.value, (j + 1) * (*pair.value));
  pair.value[j] = '\0';
  return pair;
}

kvarray_t * readKVs(const char * fname) {
  kvarray_t * pairs = malloc(sizeof(*pairs));
  pairs->kvlen = 0;
  pairs->kvarray = NULL;
  size_t sz = 0;
  ssize_t len = 0;
  char * line = NULL;
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "Can not open file %s", fname);
    exit(EXIT_FAILURE);
  }
  while ((len = getline(&line, &sz, f)) >= 0) {
    pairs->kvlen++;
    pairs->kvarray = realloc(pairs->kvarray, pairs->kvlen * sizeof(*pairs->kvarray));
    pairs->kvarray[pairs->kvlen - 1] = parseKVline(line);
    line = NULL;
  }
  free(line);
  return pairs;
}

void freeKVs(kvarray_t * pairs) {
  for (size_t i = 0; i < pairs->kvlen; i++) {
    free(pairs->kvarray[i].value);
    free(pairs->kvarray[i].key);
  }
  free(pairs->kvarray);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  for (size_t i = 0; i < pairs->kvlen; i++) {
    printf("key = '%s' value = '%s'\n", pairs->kvarray[i].key, pairs->kvarray[i].value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  for (size_t i = 0; i < pairs->kvlen; i++) {
    if (!strcmp(pairs->kvarray[i].key, key)) {
      return pairs->kvarray[i].value;
    }
  }
  return NULL;
}
