#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  pthread_mutex_t* mutex;
  pthread_cond_t* condvar_1;
  pthread_cond_t* condvar_2;
  int64_t A;
  int64_t B;
  int32_t N;
  int64_t value;
} data_t;

bool IsPrime(int64_t value) {
  for (int64_t i = 3; i < (value + 1) / 2; i += 2) {
    if (value % i == 0) {
      return false;
    }
  }
  return true;
}

static void* thread_func(void* args) {
  data_t* local = (data_t*)args;
  int64_t counter = 0;

  int64_t number = local->A % 2 == 0 ? local->A + 1 : local->A;
  while (counter < local->N && number <= local->B) {
    while (!IsPrime(number)) {
      number += 2;
    }
    local->value = number;

    pthread_cond_signal(local->condvar_1);
    pthread_cond_wait(local->condvar_2, local->mutex);
    ++counter;
    number += 2;
  }
  return NULL;
}

int main(int argc, char* args[]) {
  int64_t A = strtoll(args[1], NULL, 10);
  int64_t B = strtoll(args[2], NULL, 10);
  int32_t N = (int32_t)strtol(args[3], NULL, 10);

  pthread_cond_t condvar_1 = PTHREAD_COND_INITIALIZER;
  pthread_cond_t condvar_2 = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  data_t params = {&mutex, &condvar_1, &condvar_2, A, B, N, -1};

  pthread_mutex_lock(&mutex);
  pthread_t thread;
  pthread_create(&thread, NULL, thread_func, (void*)&params);

  for (int32_t k = 0; k < N; ++k) {
    pthread_cond_wait(&condvar_1, &mutex);

    printf("%ld\n", params.value);

    pthread_cond_signal(&condvar_2);
  }
  pthread_mutex_unlock(&mutex);
  pthread_join(thread, NULL);
  return 0;
}