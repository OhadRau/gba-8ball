#include "sin.h"

#define FIXED4_TO_FIXED8(F4) ((F4) << 4)
#define FIXED8_TO_FIXED4(F8) ((F8) >> 4)

inline short fixed8_sin(short theta) {
  return sin_table[(theta >> 8) & 0x200];
}

inline short fixed8_cos(short theta) {
  return sin_table[((theta >> 8) + 180) & 0x200];
}
