#include <stdint.h>
#include <stdio.h>
#include <windows.h>

int main(int args, char* argv[]) {
  const char kSpace = ' ';
  DWORD dw = 0;
  HANDLE input = CreateFileA(argv[1], GENERIC_READ, dw, NULL, OPEN_EXISTING,
                             FILE_ATTRIBUTE_READONLY, NULL);
  if (input == INVALID_HANDLE_VALUE) {
    DeleteFileA(argv[1]);
    return 1;
  }
  int value = -1;
  uint32_t next_pointer = 10;
  DWORD read = 0;
  ReadFile(input, &value, sizeof(value), &read, NULL);
  if (read == 0) {
    DeleteFileA(argv[1]);
    return 0;
  }
  ReadFile(input, &next_pointer, sizeof(next_pointer), &read, NULL);
  while (next_pointer > 0) {
    char buffer[20];
    sprintf(buffer, "%d", value);
    for (int i = 0; *(buffer + i); ++i) {
      write(1, buffer + i, sizeof(char));
    }
    write(1, &kSpace, sizeof(char));
    SetFilePointer(input, next_pointer, NULL, FILE_BEGIN);
    ReadFile(input, &value, sizeof(value), NULL, NULL);
    ReadFile(input, &next_pointer, sizeof(next_pointer), NULL, NULL);
  }
  char buffer[20];
  sprintf(buffer, "%d", value);
  for (int i = 0; *(buffer + i); ++i) {
    write(1, buffer + i, sizeof(char));
  }
  write(1, &kSpace, sizeof(char));
  DeleteFileA(argv[1]);
  return 0;
}