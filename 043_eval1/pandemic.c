#include "pandemic.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* Cite below function: Page 194 of AoP */
/*
void copyString(char * dest, const char * src, size_t n) {
  char * p1 = dest;
  const char * p2 = src;
  char * stop = dest + n;
  while (*p2 != '\0' && p1 != stop) {
    *p1 = *p2;
    p1++;
    p2++;
  }
  if (p1 != stop) {
    *p1 = '\0';
  }
  else {
    fprintf(stderr, "Too many characters in the line.\n");
    exit(EXIT_FAILURE);
  }
}
*/

country_t parseLine(char * line) {
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
    fprintf(stderr, "Negative number of cases.\n");
    exit(EXIT_FAILURE);
  }
  /*
  char str[21];  // length 21 to ULLONG_MAX = 18446744073709551615\0
  copyString(str, line + i, 21);
  */
  /* Cite below part: manual page of strtol */
  char * str = line + i;
  char * end;
  errno = 0;
  // convert numerical characters into unsigned long long int
  ans.population = strtoull(str, &end, 10);
  if ((errno == ERANGE && (ans.population == ULLONG_MAX || ans.population == 0)) ||
      (errno != 0 && ans.population == 0)) {
    fprintf(stderr, "strtoull can not convert string to cases number.\n");
    perror("strtoull");
    exit(EXIT_FAILURE);
  }
  if (end == str) {
    fprintf(stderr, "No cases number digits were found.\n");
    exit(EXIT_FAILURE);
  }
  if (*end != '\0' && *end != '\n') {
    // There is a newline after everything.
    fprintf(stderr, "Further characters after cases number %s in %s\n", end, str);
    exit(EXIT_FAILURE);
  }
  /* Cite above part: manual page of strtol */
  return ans;
}

void calcRunningAvg(unsigned * data, size_t n_days, double * avg) {
  if (data == NULL || n_days < 7) {
    fprintf(stderr, "n_days less than 7.\n");
    exit(EXIT_FAILURE);
  }
  double total = 0;
  double x = 0;
  for (size_t i = 0; i < (n_days - 6); i++) {
    for (size_t j = 0; j <= 6; j++) {
      if (data[i + j] < 0) {
        fprintf(stderr, "Negative population on data[%zu]", i + j);
        exit(EXIT_FAILURE);
      }
      x = (double)(data[i + j]) / 7;  // 32-bit int casting to 64-bit double
      if (x > 0 && (total + x) <= total) {
        fprintf(stderr, "double Avg[%zu] overflows.\n", i);
        exit(EXIT_FAILURE);
      }
      else {
        total += x;
      }
    }
    avg[i] = total;
    total = 0;
  }
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
