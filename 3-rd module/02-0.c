#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  pthread_mutex_t* mutex;
  double* array;
  int i;
  int N;
  int k;
} data_t;

void* thread_func(data_t* data) {
  pthread_mutex_lock(data->mutex);
  double* array = data->array;
  int i = data->i;
  int N = data->N;
  int k = data->k;
  for (int j = 0; j < N; ++j) {
    array[(i - 1 + k) % k] += 0.99;
    array[i] += 1;
    array[(i + 1) % k] += 1.01;
  }
  pthread_mutex_unlock(data->mutex);
  return NULL;
}

int main(int argc, char* argv[]) {
  int N = (int)strtol(argv[1], NULL, 10);
  int k = (int)strtol(argv[2], NULL, 10);

  double* array = (double*)calloc(k, sizeof(double));
  pthread_t* threads = (pthread_t*)malloc(k * sizeof(pthread_t));

  data_t* args = (data_t*)malloc(k * sizeof(data_t));

  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  for (int i = 0; i < k; ++i) {
    args[i].mutex = &mutex;
    args[i].array = array;
    args[i].i = i;
    args[i].N = N;
    args[i].k = k;
    pthread_create(&threads[i], NULL, (void* (*)(void*))thread_func,
                   (void*)&args[i]);
  }

  for (int i = 0; i < k; ++i) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < k; ++i) {
    printf("%.10g\n", array[i]);
  }
  free(args);
  free(threads);
  free(array);

  return 0;
}