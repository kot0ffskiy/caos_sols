#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void PutInString(char* buff, unsigned item, unsigned W) {
  char tmp = buff[W];
  sprintf(buff, "%*d", W, item);
  buff[W] = tmp;
}

void Fill(unsigned W, unsigned N, char* arr) {
  int space = 0;
  int curr_num = 1;
  int curr_size = N;
  for (; curr_size > 2; curr_size -= 2, space++) {
    for (int i = 0; i < curr_size; i++) {
      PutInString(arr + W * (N * space + space + i) + space, curr_num + i, W);
    }
    for (int i = 0; i < curr_size - 2; i++) {
      PutInString(arr + W * (N * (space + 1 + i) + space + curr_size - 1) + 1 +
                      i + space,
                  curr_num + curr_size + i, W);
    }
    for (int i = 0; i < curr_size; i++) {
      PutInString(arr + W * (N * (space + curr_size - 1) + space + i) +
                      curr_size - 1 + space,
                  curr_num + 3 * (curr_size - 1) - i, W);
    }
    for (int i = 0; i < curr_size - 2; i++) {
      PutInString(arr + W * (N * (space + 1 + i) + space) + 1 + i + space,
                  curr_num + 4 * (curr_size - 1) - 1 - i, W);
    }
    curr_num += 4 * (curr_size - 1);
  }
  if (curr_size == 1) {
    PutInString(arr + W * (N * space + space) + space, curr_num, W);
  } else if (curr_size == 2) {
    PutInString(arr + W * (N * space + space) + space, curr_num, W);
    PutInString(arr + W * (N * space + space + 1) + space, curr_num + 1, W);
    PutInString(arr + W * (N * (space + 1) + space + 1) + 1 + space,
                curr_num + 2, W);
    PutInString(arr + W * (N * (space + 1) + space) + 1 + space, curr_num + 3,
                W);
  }
}

int main(int args, char* argv[]) {
  int input = open(argv[1], O_RDWR | O_CREAT, 0640);
  unsigned size = 0;
  int dec = 1;
  unsigned long k = strlen(argv[2]);
  while (argv[2][--k] != 0) {
    size += (argv[2][k] - '0') * dec;
    dec *= 10;
  }
  unsigned wide = 0;
  dec = 1;
  k = strlen(argv[3]);
  while (argv[3][--k] != 0) {
    wide += (argv[3][k] - '0') * dec;
    dec *= 10;
  }
  int file_size = size * wide * size + size;
  ftruncate(input, file_size);
  char* curr =
      mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, input, 0);
  Fill(wide, size, curr);
  for (int i = 0; i < size; ++i) {
    curr[wide * (size * i + size) + i] = '\n';
  }
  msync(curr, file_size, MS_SYNC);
  munmap(curr, file_size);
  close(input);
  return 0;
}