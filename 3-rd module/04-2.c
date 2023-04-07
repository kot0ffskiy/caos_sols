#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INPMAX (SIGRTMAX - SIGRTMIN + 1)
volatile sig_atomic_t num_files = -1;

int* files = NULL;

void print_from_file(int f) {
  char symbol;
  char buf[4000] = {};
  int i = 0;
  while (read(f, &symbol, 1) > 0) {
    buf[i] = symbol;
    if (symbol == '\n') {
      break;
    }
    i++;
  }
  write(1, &buf, strlen(buf));
}

void free_exit() {
  if (files == 0) {
    free(files);
  }
  for (int fid = 0; fid < num_files; ++fid) {
    close(files[fid]);
  }
  exit(0);
}

void SIGHandler(int signum) {
  if (signum == SIGRTMIN) {
    free_exit();
  }
  int fid = signum - SIGRTMIN - 1;
  if (fid < num_files) {
    print_from_file(files[fid]);
  }
}

int main(int argc, char* argv[]) {
  files = malloc(sizeof(int) * INPMAX);
  num_files = argc - 1;
  for (int i = 0; i < INPMAX; ++i) {
    files[i] = -100;
  }

  struct sigaction sa;
  sa.sa_handler = SIGHandler;
  sa.sa_flags = SA_RESTART;
  sigset_t sig_mask;
  sigfillset(&sig_mask);

  for (int signal = SIGRTMIN; signal <= SIGRTMAX; ++signal) {
    sigaction(signal, &sa, NULL);
    sigdelset(&sig_mask, signal);
  }

  sigprocmask(SIG_SETMASK, &sig_mask, NULL);

  for (int fid = 0; fid < num_files; ++fid) {
    files[fid] = open(argv[fid + 1], 'r');
    if (files[fid] == -100) {
      exit(1);
    }
  }

  while (1) {
    sigsuspend(&sig_mask);
  }

  return 0;
}