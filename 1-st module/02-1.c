#include <stdint.h>

extern void sum(ITYPE first, ITYPE second, ITYPE* res, int* CF) {
  ITYPE carry = 0;
  ITYPE size = sizeof(ITYPE);
  for (ITYPE i = 0; i < size; ++i) {
    carry = first & second;
    first = first ^ second;
    second = (carry << 1);
  }
  *res = first;
  if (carry == 0 && *CF != 1) {
    *CF = 0;
  } else {
    *CF = 1;
  }
}

extern void mul(ITYPE first, ITYPE second, ITYPE* res, int* CF) {
  *CF = 0;
  if (first == 0 || second == 0) {
    *res = 0;
    return;
  } else {
    *res = first;
  }
  while (second != 1) {
    sum(first, *res, res, CF);
    second--;
  }
}
