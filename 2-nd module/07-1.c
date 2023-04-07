#include <sys/syscall.h>

long syscall(long number, ...);

void _start() {
  char hello;
  int intret = syscall(SYS_read, 0, &hello, 1);
  while (intret != 0) {
    syscall(SYS_write, 1, &hello, 1);
    intret = syscall(SYS_read, 0, &hello, 1);
  }
  syscall(SYS_exit, 0);
}