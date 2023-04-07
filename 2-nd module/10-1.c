#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

struct Item {
  int value;
  uint32_t next_pointer;
};

int main(int args, char* argv[]) {
  int input = open(argv[1], O_RDONLY);
  if (input < 0) {
    close(input);
    return 1;
  }
  struct stat st;
  fstat(input, &st);
  size_t size = st.st_size;
  if (size == 0) {
    close(input);
    return 0;
  }
  struct Item* curr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, input, 0);
  struct Item* start = curr;
  while (1) {
    printf("%d ", curr->value);
    if (curr->next_pointer == 0) {
      break;
    }
    curr = start + (curr->next_pointer / sizeof(struct Item));
  }
  close(input);
  return 0;
}