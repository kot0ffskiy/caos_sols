#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
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
  int file = open("1-12-1.c", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  snprintf(
      prog, sizeof(prog),
      "#include <stdio.h>\n int main() {\n int param = (%s);\n printf(\"%%d\", "
      "param);\n "
      "return 0;\n }",
      buf);
  write(file, prog, strnlen(prog, sizeof(prog)));
  close(file);
  pid_t pid = fork();
  if (pid == 0) {
    execlp("clang", "clang", "1-12-1.c", "-o", "1-12-1", NULL);
    exit(0);
  } else {
    waitpid(pid, NULL, 0);
  }
  pid = fork();
  if (pid == 0) {
    execlp("./1-12-1", "./1-12-1", NULL);
    exit(0);
  } else {
    waitpid(pid, NULL, 0);
  }
  remove("1-12-1.c");
  remove("1-12-1");

  return 0;
}