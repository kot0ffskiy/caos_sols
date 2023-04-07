#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main(int args, char* argv[]) {
  int input = open(argv[1], O_RDONLY);
  if (input < 0) {
    close(input);
    return 1;
  }
  int value = -1;
  uint32_t next_pointer = 10;
  const char kSpace = ' ';
  while (next_pointer > 0) {
    if (read(input, &value, sizeof(value)) <= 0) {
      close(input);
      return 0;
    }
    char buffer[20];
    sprintf(buffer, "%d", value);
    for (int i = 0; *(buffer + i); ++i) {
      write(1, buffer + i, sizeof(char));
    }
    write(1, &kSpace, sizeof(char));
    read(input, &next_pointer, sizeof(next_pointer));
    lseek(input, next_pointer, SEEK_SET);
  }
  close(input);
  return 0;
}