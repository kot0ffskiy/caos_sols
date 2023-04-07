#include <stdint.h>
#include <string.h>

typedef enum {
  PlusZero = 0x00,
  MinusZero = 0x01,
  PlusInf = 0xF0,
  MinusInf = 0xF1,
  PlusRegular = 0x10,
  MinusRegular = 0x11,
  PlusDenormal = 0x20,
  MinusDenormal = 0x21,
  SignalingNaN = 0x30,
  QuietNaN = 0x31
} float_class_t;

extern float_class_t classify(double* value_ptr) {
  uint64_t a;
  memcpy(&a, value_ptr, sizeof(uint64_t));
  uint64_t mantiss = a & 0b1111111111111111111111111111111111111111111111111111;
  uint64_t pordok = (a >> 52) & 0b11111111111;
  uint64_t znak = a >> 63;
  if (znak > 0) {
    if (pordok == 0b11111111111 && mantiss == 0) {
      return MinusInf;
    }
    if (pordok == 0 && mantiss == 0) {
      return MinusZero;
    }
    if (pordok == 0) {
      return MinusDenormal;
    }
    return MinusRegular;
  }
  if (znak == 0) {
    if (pordok == 0b11111111111) {
      if (mantiss == 0) {
        return PlusInf;
      }
      if (mantiss >= 0b1000000000000000000000000000000000000000000000000000) {
        return QuietNaN;
      }
      return SignalingNaN;
    }
    if (pordok == 0) {
      if (mantiss == 0) {
        return PlusZero;
      }
      return PlusDenormal;
    }
    return PlusRegular;
  }
  return 0;
}
