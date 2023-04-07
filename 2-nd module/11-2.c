#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int args, char* argv[]) {
  char buff[4096];
  int wordcount = 0;
  while (1) {
    pid_t new_proc = fork();
    if (new_proc == 0) {
      exit(scanf("%s", buff) != EOF);
    } else {
      int status;
      waitpid(new_proc, &status, 0);
      if (status == 0) {
        break;
      }
      wordcount++;
    }
  }
  printf("%d", wordcount);
  return 0;
}