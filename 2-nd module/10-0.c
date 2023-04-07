#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int filename = open(argv[1], O_RDONLY);
  struct stat st;
  fstat(filename, &st);
  unsigned long size = st.st_size;
  char* contents = mmap(NULL, size, PROT_READ, MAP_PRIVATE, filename, 0);

  char* entry = contents;
  while ((entry = memmem(entry, size, argv[2], strlen(argv[2]))) != NULL) {
    unsigned long long diff = entry - contents;
    size = st.st_size - diff - 1;
    printf("%lu ", diff);
    ++entry;
  }

  munmap(contents, st.st_size);
  close(filename);
  return 0;
}