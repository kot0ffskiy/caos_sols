#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t n = 0;

void handler(int sig, siginfo_t* si, void* unused) {
  if (sig == SIGRTMIN) {
    n = si->si_value.sival_int;
    if (n == 0) {
      // printf("Received N=0, exiting...\n");
      // fflush(stdout);
      exit(EXIT_SUCCESS);
    }
    // printf("Received N=%d\n", n);
    // fflush(stdout);
    n--;
    union sigval data = {.sival_int = n};
    if (sigqueue(si->si_pid, SIGRTMIN, data) == -1) {
      perror("sigqueue");
      exit(EXIT_FAILURE);
    }
  }
}

int main() {
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = handler;
  if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
    perror("sigaction");
    return EXIT_FAILURE;
  }
  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask, SIGRTMIN);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
    perror("sigprocmask");
    return EXIT_FAILURE;
  }
  // printf("%d\n", getpid());
  // printf("Waiting for signals...\n");
  fflush(stdout);
  while (1) {
    sigsuspend(&mask);
  }
  return 0;
}