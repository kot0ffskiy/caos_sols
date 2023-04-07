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
  char* cmd1 = argv[1];
  char* cmd2 = argv[2];
  int fds_pair[2];
  if (pipe(fds_pair) < 0) {
    perror("pipe error");
    exit(1);
  }
  pid_t pid1 = fork();
  if (pid1 == -1) {
    perror("fork error");
    exit(1);
  }
  pid_t pid2 = -10;
  if (pid1 != 0) {
    pid2 = fork();
    if (pid2 == -1) {
      perror("fork error");
      exit(1);
    }
  }
  if (pid1 == 0) {
    dup2(fds_pair[1], 1);
    close(fds_pair[1]);
    if (execlp(cmd1, cmd1, NULL) == -1) {
      perror("exec error");
      exit(1);
    }
  }
  if (pid2 == 0) {
    close(fds_pair[1]);
    dup2(fds_pair[0], 0);
    close(fds_pair[0]);
    if (execlp(cmd2, cmd2, NULL) == -1) {
      perror("exec error");
      exit(1);
    }
  }
  close(fds_pair[1]);
  waitpid(pid1, 0, 0);
  waitpid(pid2, 0, 0);
  return 0;
}