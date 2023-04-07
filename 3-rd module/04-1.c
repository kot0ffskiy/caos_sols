#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

volatile sig_atomic_t counter = 0;
volatile sig_atomic_t exit_trigger = 0;
volatile sig_atomic_t value = 0;

void SIGUSR1Handler() { value++; }

void SIGUSR2Handler() { value *= -1; }

void SIGINTHandler() { exit_trigger = 1; }

void SIGTERMHandler() { exit_trigger = 1; }

int main() {
  struct sigaction sigusr1_handler;
  memset(&sigusr1_handler, 0, sizeof(sigusr1_handler));
  sigusr1_handler.sa_handler = SIGUSR1Handler;
  sigusr1_handler.sa_flags = SA_RESTART;
  sigaction(SIGUSR1, &sigusr1_handler, NULL);

  struct sigaction sigusr2_handler;
  memset(&sigusr2_handler, 0, sizeof(sigusr2_handler));
  sigusr2_handler.sa_handler = SIGUSR2Handler;
  sigusr2_handler.sa_flags = SA_RESTART;
  sigaction(SIGUSR2, &sigusr2_handler, NULL);

  struct sigaction sigint_handler;
  memset(&sigint_handler, 0, sizeof(sigint_handler));
  sigint_handler.sa_handler = SIGINTHandler;
  sigint_handler.sa_flags = SA_RESTART;
  sigaction(SIGINT, &sigint_handler, NULL);

  struct sigaction sigterm_handler;
  memset(&sigterm_handler, 0, sizeof(sigterm_handler));
  sigterm_handler.sa_handler = SIGTERMHandler;
  sigterm_handler.sa_flags = SA_RESTART;
  sigaction(SIGTERM, &sigterm_handler, NULL);

  printf("%d\n", getpid());
  fflush(stdout);

  scanf("%d", &value);
  fflush(stdin);

  while (!exit_trigger) {
    pause();
    if (exit_trigger) {
      break;
    }
    printf("%d\n", value);
    fflush(stdout);
  }

  return 0;
}