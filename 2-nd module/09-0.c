#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

const uint16_t PATH_MAX = 4096;

int main(int argc, char* argv[]) {
  char file_name[PATH_MAX];
  struct stat st;
  uint64_t result = 0;
  while (fgets(file_name, PATH_MAX, stdin) != NULL) {
    strtok(file_name, "\n");
    if (lstat(file_name, &st) != -1 && S_ISREG(st.st_mode)) {
      result += st.st_size;
    }
  }
  printf("%lu\n", result);
}