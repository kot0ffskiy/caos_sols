#include <stdint.h>
#include <stdio.h>

#define ALPHABET 26
#define DIGITS 10

int main() {
  uint64_t result = 0;
  uint64_t hold_char = 0;
  int input;
  while (EOF != (input = getchar())) {
    /*/
     битовая лента. запись справа налево: цифры->заглавные->маленькие
    /*/
    if ('0' <= input && '9' >= input) {
      hold_char = hold_char | (1ULL << (input - '0'));
    }
    if ('a' <= input && 'z' >= input) {
      hold_char = hold_char | (1ULL << (input - 'a' + DIGITS + ALPHABET));
    }
    if ('A' <= input && 'Z' >= input) {
      hold_char = hold_char | (1ULL << (input - 'A' + DIGITS));
    }
    if (input == '|') {
      result = result | hold_char;
      hold_char = 0;
    }
    if (input == '^') {
      result = result ^ hold_char;
      hold_char = 0;
    }
    if (input == '&') {
      result = result & hold_char;
      hold_char = 0;
    }
    if (input == '~') {
      result = ~result;
    }
  }
  int res = '1';
  for (int i = 0; i < (2 * ALPHABET + DIGITS); i++) {
    if ((i >= DIGITS + ALPHABET) && (i < DIGITS + 2 * ALPHABET)) {
      res = i - ALPHABET - DIGITS + 'a';
    }
    if (i >= DIGITS && i < DIGITS + ALPHABET) {
      res = i - DIGITS + 'A';
    }
    if (i < DIGITS) {
      res = (i + '0');
    }
    if (((result >> i) & 1)) {
      printf("%c", res);
    }
  }
  return 0;
}