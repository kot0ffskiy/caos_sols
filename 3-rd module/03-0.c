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
  char* cmd = argv[1];
  char* file = argv[2];
  int in = open(file, O_RDONLY);
  dup2(in, 0);
  close(in);
  int fds_pair[2];
  if (pipe(fds_pair) < 0) {
    perror("pipe error");
    exit(1);
  }
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork error");
    exit(1);
  }
  if (pid == 0) {
    dup2(fds_pair[1], 1);
    close(fds_pair[1]);
    if (execlp(cmd, cmd, NULL) == -1) {
      perror("exec error");
      exit(1);
    }
  } else {
    close(fds_pair[1]);
    uint64_t total_count = 0;
    ssize_t count;
    char buff[4096];
    while ((count = read(fds_pair[0], buff, sizeof(buff))) > 0) {
      total_count += count;
    }
    waitpid(pid, 0, 0);
    printf("%lu\n", total_count);
  }
  return 0;
}