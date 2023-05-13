// ITMO 09.03.02 2020
// Artem Mikheev M3101
// variant 17
// lab 12

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf(
      "Usage:\n"
      "%s filepath\n");
    return 0;
  }

  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("{Error} Failed opening specified file");
    return 0;
  }

  puts("Lines of file on even positions:");

  int print_this = 0;
  char *buffer = NULL;
  size_t read_bytes, len;
  while (!feof(file)) {
    if ( (len = getline(&buffer, &read_bytes, file)) < 0 && ferror(file) ) {
      perror("Error reading from file");
      free(buffer);
      break;
    }
    if (print_this) {
		buffer[len - 1] = '\0';
		puts(buffer);
	}
    print_this = !print_this;
  }
  free(buffer);
  fclose(file);
  return 0;
}
