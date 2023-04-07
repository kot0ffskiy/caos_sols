#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int args, char* argv[]) {
  int N = strtol(argv[1], NULL, 10);

  for (int i = 0; i < N; ++i) {
    pid_t proc = fork();
    if (proc == 0) {
      if (i == N - 1) {
        printf("%d\n", i + 1);
      } else {
        printf("%d ", i + 1);
      }
      exit(0);
    }
    waitpid(proc, NULL, 0);
  }
  return 0;
}