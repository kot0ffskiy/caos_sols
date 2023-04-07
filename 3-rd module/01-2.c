#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
  int input_num;
  int sockets[2];
  bool is_first;
  bool is_last;
} state;

static void* thread_func_1(void* arg) {
  state* st = (state*)arg;
  int num;
  while (1) {
    if (st->is_last) {
      return NULL;
    }
    if (st->is_first) {
      num = st->input_num;
      st->is_first = false;
    } else {
      read(st->sockets[1], &num, sizeof(num));
    }
    num -= 3;
    if (num > 100 || num == 0) {
      st->is_last = true;
      printf("%d\n", num);
      write(st->sockets[1], &num, sizeof(num));
      return NULL;
    } else {
      if (st->is_last) {
        return NULL;
      }
      printf("%d\n", num);
      write(st->sockets[1], &num, sizeof(num));
    }
  }
}

static void* thread_func_2(void* arg) {
  state* st = (state*)arg;
  int num;
  while (1) {
    if (st->is_last) {
      return NULL;
    }
    read(st->sockets[0], &num, sizeof(num));

    num += 5;
    if (num > 100 || num == 0) {
      st->is_last = true;
      printf("%d\n", num);
      write(st->sockets[0], &num, sizeof(num));
      return NULL;
    } else {
      if (st->is_last) {
        return NULL;
      }
      printf("%d\n", num);
      write(st->sockets[0], &num, sizeof(num));
    }
  }
}

int main(int argc, char* args[]) {
  state st;
  st.input_num = strtol(args[1], NULL, 10);
  st.is_first = true;
  st.is_last = false;
  socketpair(AF_UNIX, SOCK_STREAM, 0, st.sockets);
  pthread_t thread[2];
  pthread_create(&thread[0], NULL, thread_func_1, &st);
  pthread_create(&thread[1], NULL, thread_func_2, &st);
  pthread_join(thread[0], NULL);
  pthread_join(thread[1], NULL);
  return 0;
}