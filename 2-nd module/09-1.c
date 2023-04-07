#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const uint16_t PATH_MAX = 4096;

int main(int argc, char* argv[]) {
  char file_name[PATH_MAX];
  while (fgets(file_name, PATH_MAX, stdin) != NULL) {
    strtok(file_name, "\n");
    if (access(file_name, X_OK) == 0) {
      int fstr = open(file_name, O_RDONLY);
      if (fstr < 0) {
        continue;
      }
      char word[4];
      read(fstr, word, sizeof(char) * 4);
      if (!((word[0] == 127 && word[1] == 'E' && word[2] == 'L' &&
             word[3] == 'F'))) {
        if ((word[0] == '#' && word[1] == '!')) {
          char check[PATH_MAX];
          lseek(fstr, 2, SEEK_SET);
          read(fstr, check, sizeof(check));
          strtok(check, "\n");
          struct stat st;
          if (!(lstat(check, &st) != -1 && S_ISREG(st.st_mode) &&
                access(check, X_OK) == 0)) {
            printf("%s\n", file_name);
          }
        } else {
          printf("%s\n", file_name);
        }
      }
      close(fstr);
    }
  }
}
