#include "counts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

counts_t * createCounts(void) {
  counts_t * c = malloc(sizeof(*c));
  c->len = 0;
  c->unknownNum = 0;
  c->counts = malloc(c->len * sizeof(*c->counts));
  return c;
}

void addCount(counts_t * c, const char * name) {
  if (name == NULL) {
    c->unknownNum++;
    return;
  }
  for (size_t i = 0; i < c->len; i++) {
    if (!strcmp(name, c->counts[i].value)) {
      c->counts[i].num++;
      return;
    }
  }
  // New value add to c;
  c->len++;
  c->counts = realloc(c->counts, c->len * sizeof(*c->counts));
  c->counts[c->len - 1].value = malloc((strlen(name) + 1) * sizeof(char));
  c->counts[c->len - 1].value =
      strncpy(c->counts[c->len - 1].value, name, strlen(name) + 1);
  c->counts[c->len - 1].num = 1;
}

void printCounts(counts_t * c, FILE * outFile) {
  for (size_t i = 0; i < c->len; i++) {
    fprintf(outFile, "%s: %d\n", c->counts[i].value, c->counts[i].num);
  }
  if (c->unknownNum != 0) {
    fprintf(outFile, "<unknown> : %d\n", c->unknownNum);
  }
}

void freeCounts(counts_t * c) {
  for (size_t i = 0; i < c->len; i++) {
    free(c->counts[i].value);
  }
  free(c->counts);
  free(c);
}
