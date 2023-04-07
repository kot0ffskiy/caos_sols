#include <limits.h>
#include <stdio.h>

extern void normalize_path(char* string) {
  int newcount = 0, oldcount = 0, lastback = 0;
  char i_found_backslash = 0;
  while (*(string + oldcount) != '\0') {
    while (*(string + oldcount) == '/' && *(string + oldcount + 1) == '/') {
      oldcount++;
      i_found_backslash = 1;
    }
    if (i_found_backslash == 1) {
      newcount++;
      oldcount++;
      *(string + newcount++) = *(string + oldcount++);
      i_found_backslash = 0;
      continue;
    }
    if (*(string + oldcount) == '.' && *(string + oldcount + 1) == '/') {
      *(string + newcount) = *(string + oldcount + 1);
      oldcount += 2;
      continue;
    }
    if (*(string + oldcount) == '/' && *(string + oldcount + 1) != '.') {
      i_found_backslash = oldcount;
    }
    if (*(string + oldcount) == '.' && *(string + oldcount + 1) == '.' &&
        *(string + oldcount + 2) == '/') {
      newcount = i_found_backslash;
      oldcount += 3;
      *(string + ++newcount) = *(string + oldcount - 1);
      continue;
    }
    *(string + newcount++) = *(string + oldcount++);
  }
  *(string + newcount) = '\0';
}