#include "pandemic.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

country_t parseLine(char * line) {
  /* 
     This function is to parse line
     and to return a struct with country name and population in that line.  
  */
  country_t ans;
  ans.name[0] = '\0';
  ans.population = 0;
  if (line == NULL) {
    fprintf(stderr, "Empty line.\n");
    exit(EXIT_FAILURE);
  }
  // Parse country name part
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
  // Parse population number  part
  i++;
  if (line[i] == '-') {
    fprintf(stderr, "Negative number of cases.\n");
    exit(EXIT_FAILURE);
  }

  /* Cite below part: manual page of strtol */
  char * str = line + i;
  char * end;
  errno = 0;
  // Convert numerical characters into unsigned long long int
  // ULLONG_MAX = 2^64-1  = 18446744073709551615
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
  /*
    This function is to calculate the seven-day running average of case data for each cuntry.
  */
  if (data == NULL || n_days < 7) {
    fprintf(stderr, "n_days less than 7.\n");
    exit(EXIT_FAILURE);
  }
  double total;
  double x;
  for (size_t i = 0; i < (n_days - 6); i++) {
    total = 0;
    for (size_t j = 0; j < 7; j++) {
      x = (double)(data[i + j]) / 7;  // 32-bit int casting to 64-bit double
      if (x > 0 && (total + x) <= total) {
        fprintf(stderr, "Overflow caused at double Avg[%zu].\n", i);
        exit(EXIT_FAILURE);
      }
      else {
        total += x;
      }
    }
    avg[i] = total;
  }
}

void calcCumulative(unsigned * data, size_t n_days, uint64_t pop, double * cum) {
  if (data == NULL || n_days == 0) {
    fprintf(stderr, "Empty line.\n");
    exit(EXIT_FAILURE);
  }
  if (pop == 0) {
    fprintf(stderr, "Zero population.\n");
    exit(EXIT_FAILURE);
  }
  double density;
  cum[0] = data[0] / (double)pop * 100000;
  for (size_t i = 1; i < n_days; i++) {
    density = data[i] / (double)pop * 100000;
    if (density > 0 && (cum[i - 1] + density) <= cum[i - 1]) {
    }
    else {
      cum[i] = cum[i - 1] + density;
    }
  }
}

void printCountryWithMax(country_t * countries,
                         size_t n_countries,
                         unsigned ** data,
                         size_t n_days) {
  if (countries == NULL || n_countries == 0 || data == NULL || n_days == 0) {
    fprintf(stderr, "Empty data.\n");
    exit(EXIT_FAILURE);
  }
  // Array of country's index with max cases for each days
  size_t country_idx_days[n_days];
  for (size_t i = 0; i < n_days; i++) {
    size_t country_idx = 0;
    for (size_t j = 1; j < n_countries; j++) {
      if (data[country_idx][i] < data[j][i]) {
        country_idx = j;
      }
    }
    // Country's index with max cases in day i
    country_idx_days[i] = country_idx;
  }
  size_t day_idx = 0;
  unsigned max = data[country_idx_days[0]][0];
  unsigned curr;
  for (size_t i = 1; i < n_days; i++) {
    curr = data[country_idx_days[i]][i];
    if (max < curr) {
      day_idx = i;
      max = curr;
    }
  }
  printf("%s has the most daily cases with %u\n",
         countries[country_idx_days[day_idx]].name,
         max);
}
