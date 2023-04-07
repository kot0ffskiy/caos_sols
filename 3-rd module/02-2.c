#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Item {
  struct Item* _Atomic next;
  int64_t value;
} item_t;

typedef struct {
  item_t* head;
  int64_t k;
  int64_t N;
  int64_t number;
} Params;

void* thread_func(void* args) {
  Params* _Atomic input = (Params*)args;

  for (int64_t i = input->k * input->number;
       i < (input->k) * (input->number + 1); ++i) {
    item_t* _Atomic new = (item_t*)malloc(sizeof(item_t));
    new->next = atomic_exchange(&input->head->next, new);
    new->value = i;
  }

  return NULL;
}

int main(int argc, char* args[]) {
  int64_t N = (int64_t)strtoll(args[1], NULL, 10);
  int64_t k = (int64_t)strtoll(args[2], NULL, 10);

  pthread_t thread[N];
  item_t* head = (item_t*)malloc(sizeof(item_t));
  item_t* end = (item_t*)malloc(sizeof(item_t));
  head->value = -1;
  end->value = -2;
  head->next = end;
  end->next = NULL;
  Params input[N];
  for (int64_t i = 0; i < N; ++i) {
    input[i].head = head;
    input[i].k = k;
    input[i].N = N;
    input[i].number = i;
    pthread_create(&thread[i], NULL, thread_func, (void*)&input[i]);
  }

  for (int64_t i = 0; i < N; ++i) {
    pthread_join(thread[i], NULL);
  }
  item_t* current = head->next;
  item_t* next = current->next;
  item_t* deleted = current;

  while (current != end) {
    printf("%ld\n", current->value);
    current = next;
    next = next->next;
    free(deleted);
    deleted = current;
  }
  free(head);
  free(end);
  return 0;
}