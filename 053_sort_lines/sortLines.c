#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void ReadandSort(FILE * f) {
  char ** lines = NULL;
  char * curr = NULL;
  size_t linecap;
  size_t i = 0;
  while (getline(&curr, &linecap, f) >= 0) {
    lines = realloc(lines, (i + 1) * sizeof(*lines));
    lines[i] = curr;
    curr = NULL;
    i++;
  }
  free(curr);
  sortData(lines, i);
  for (size_t j = 0; j < i; j++) {
    printf("%s", lines[j]);
    free(lines[j]);
  }
  free(lines);
}

int main(int argc, char ** argv) {
  if (argc == 1) {
    ReadandSort(stdin);
  }
  else {
    //read from each input arg file argv[i]
    FILE * f;
    for (int i = 1; i < argc; i++) {
      f = fopen(argv[i], "r");
      if (f == NULL) {
        perror("Can not open file!\n");
        return EXIT_FAILURE;
      }
      ReadandSort(f);
      if (fclose(f) != 0) {
        perror("Failed to close the input file!\n");
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}
