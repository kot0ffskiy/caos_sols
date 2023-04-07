#include <bits/local_lim.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void* thread_func(void* arg) {
  uint64_t sum = 0;
  int64_t input;
  while (scanf("%ld ", &input) != -1) {
    sum += input;
  }

  return (void*)sum;
}

int main(int argc, char* argv[]) {
  uint64_t ThreadCount = strtol(argv[1], NULL, 10);
  uint64_t res_sum = 0;
  pthread_t* threads = (pthread_t*)calloc(ThreadCount, sizeof(pthread_t));
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setguardsize(&attr, 0);
  pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN / 2);
  for (int64_t i = 0; i < ThreadCount; ++i) {
    pthread_create(&threads[i], &attr, thread_func, 0);
  }
  for (int64_t i = 0; i < ThreadCount; ++i) {
    void* res;
    pthread_join(threads[i], &res);
    res_sum += (int64_t)res;
  }
  printf("%ld", res_sum);
  pthread_attr_destroy(&attr);
  free(threads);

  return 0;
}