#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encrypt(FILE * f, int key, FILE * outfile) {
  char * line = NULL;
  size_t sz = 0;
  ssize_t len = 0;
  while ((len = getline(&line, &sz, f)) >= 0) {
    char * ptr = line;
    int i = 0;
    while (ptr[i] != '\0') {
      int c = ptr[i];
      if (isalpha(c)) {
        c = tolower(c);
        c -= 'a';
        c += key;
        c %= 26;
        c += 'a';
      }
      ptr[i] = c;
      i++;
    }
    fprintf(outfile, "%s", line);
    line = NULL;
    free(ptr);
  }
  free(line);
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: encrypt key inputFileName\n");
    return EXIT_FAILURE;
  }
  int key = atoi(argv[1]);
  if (key == 0) {
    fprintf(stderr, "Invalid key (%s): must be a non-zero integer\n", argv[1]);
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[2], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  //outfileNAme is argv[2] + ".txt", so add 4 to its length.
  // +1 for '\0' +4 for ".enc"
  char * outFileName = malloc((strlen(argv[2]) + 9) * sizeof(*outFileName));
  strcpy(outFileName, argv[2]);
  strcat(outFileName, ".enc");
  FILE * outFile = fopen(outFileName, "w");
  encrypt(f, key, outFile);
  free(outFileName);
  if (fclose(outFile) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
