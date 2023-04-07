#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  char buf[4096];
  char prog[4096];
  fgets(buf, sizeof(buf), stdin);
  char* slash_n_pos = strchr(buf, '\n');

  if (slash_n_pos != NULL) {
    *slash_n_pos = '\0';
  }

  if (strnlen(buf, sizeof(buf)) == 0) {
    return 0;
  }

  snprintf(prog, sizeof(prog), "expr = %s; print(expr)", buf);
  execlp("python3", "python3", "-c", prog, NULL);
  perror("exec");
  exit(1);
}