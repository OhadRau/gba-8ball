#ifndef LUT_H
#define LUT_H
#include "../logic.h"
#define FIXED_SIN(f) (FIXED_TO_INT(f) <= 360 ? fixed_sin_table[FIXED_TO_INT(f)] : fixed_sin_table[FIXED_TO_INT(f)%360])
#define FIXED_COS(f) (FIXED_TO_INT(f) <= 360 ? fixed_cos_table[FIXED_TO_INT(f)] : fixed_cos_table[FIXED_TO_INT(f)%360])
extern fixed_t fixed_sin_table[];
extern fixed_t fixed_cos_table[];
#endif