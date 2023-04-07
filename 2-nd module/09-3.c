#include <libgen.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const uint16_t PATH_MAX = 4096;

int main(int argc, char* argv[]) {
  char input[PATH_MAX];
  char link[PATH_MAX];
  char path[PATH_MAX];
  char res[PATH_MAX];
  while (fgets(input, sizeof(input), stdin) != NULL) {
    *(input + strlen(input) - 1) = '\0';
    struct stat st;
    int stat = lstat(input, &st);
    if (S_ISREG(st.st_mode)) {
      char link_to[10] = "link_to_";
      char* bn = basename(input);
      strcat(link_to, bn);
      symlink(input, link_to);
      continue;
    }
    if (S_ISLNK(st.st_mode)) {
      int read_link = readlink(input, link, sizeof(link));
      *(link + read_link) = '\0';
      sprintf(path, "%s/%s", dirname(input), link);
      realpath(path, res);
      printf("%s\n", res);
      continue;
    }
    if (stat == -1) {
      continue;
    }
  }
  return 0;
}