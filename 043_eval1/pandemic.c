#include "pandemic.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

country_t parseLine(char * line) {
  //WRITE ME
  country_t ans;
  ans.name[0] = '\0';
  ans.population = 0;
  if (line == NULL) {
    fprintf(stderr, "Empty line.\n");
    exit(EXIT_FAILURE);
  }
  // Country name part
  int i = 0;
  while ((line[i] != ',') && (line[i] != '\0')) {
    ans.name[i] = line[i];
    i++;
  }
  if (i > 64) {
    fprintf(stderr, "Country name length out of boundary.\n");
    exit(EXIT_FAILURE);
  }
  if (i == 0) {
    fprintf(stderr, "No country name provided in this line.\n");
    exit(EXIT_FAILURE);
  }
  if ((i != 0) && (line[i] == '\0')) {
    fprintf(stderr, "No comma separator in this line.\n");
    exit(EXIT_FAILURE);
  }
  ans.name[i] = '\0';
  // Number part
  i++;
  if (line[i] == '-') {
    fprintf(stderr, "Negative population.\n");
    exit(EXIT_FAILURE);
  }
  /* Cite below part: manual page of strtol */
  char * str = line + i;
  char * endptr = NULL;
  errno = 0;
  ans.population = strtoull(str, &endptr, 10);
  if ((errno == ERANGE && (ans.population == ULLONG_MAX || ans.population == 0)) ||
      (errno != 0 && ans.population == 0)) {
    fprintf(stderr, "strtoull can not convert string.\n");
    perror("strtoull");
    exit(EXIT_FAILURE);
  }
  if (endptr == str) {
    fprintf(stderr, "No population digits were found.\n");
    exit(EXIT_FAILURE);
  }
  /* Cite above part: manual page of strtol */
  return ans;
}

void calcRunningAvg(unsigned * data, size_t n_days, double * avg) {
  //WRITE ME
}

void calcCumulative(unsigned * data, size_t n_days, uint64_t pop, double * cum) {
  //WRITE ME
}

void printCountryWithMax(country_t * countries,
                         size_t n_countries,
                         unsigned ** data,
                         size_t n_days) {
  //WRITE ME
}
