#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  if (argc == 2) {
    execlp(argv[1], argv[1], NULL);
    return 0;
  }
  char* cmd1 = argv[1];
  int fds_pair[2];
  pipe(fds_pair);
  pid_t pid1 = fork();
  pid_t pid2 = -10;
  if (pid1 != 0) {
    pid2 = fork();
  }
  if (pid1 == 0) {
    dup2(fds_pair[1], 1);
    close(fds_pair[1]);
    execlp(cmd1, cmd1, NULL);
  }
  if (pid2 == 0) {
    close(fds_pair[1]);
    dup2(fds_pair[0], 0);
    close(fds_pair[0]);
    argv[1] = argv[0];
    execvp(argv[1], argv + 1);
  }
  close(fds_pair[1]);
  waitpid(pid1, 0, 0);
  waitpid(pid2, 0, 0);
  return 0;
}