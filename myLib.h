#ifndef MYLIB_H
#define MYLIB_H

// I don't really get what this file is supposed to contain lol
// I moved some of the stuff that's used in most places into this file but leaving some stuff in other headers where it makes sense

// --------------------- FIXED POINT MACROS & TYPES ---------------------
#define FIXED_RESOLUTION 8

#define FIXED_TO_INT(F) ((F) >> FIXED_RESOLUTION)
#define INT_TO_FIXED(I) ((I) << FIXED_RESOLUTION)
#define FIXED_MULT(A, B) (((A) * (B)) >> FIXED_RESOLUTION)
#define FIXED_DIV(A, B) (((fixed_t) (A) * (1 << FIXED_RESOLUTION)) / (B))

#define FIXED_ONE INT_TO_FIXED(1)

typedef int fixed_t;

// --------------------- MISCELLANEOUS MACROS ---------------------
#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x < y) ? y : x)

// --------------------- BIT FIELD SET/GET MACROS ---------------------
#define BF_PREP(x, name)         ( ((x)<<name##_SHIFT)& name##_MASK  )
#define BF_GET(x, name)          ( ((x) & name##_MASK)>> name##_SHIFT )
#define BF_SET(y, x, name)       (y = ((y)&~name##_MASK) | BF_PREP(x,name) )

#define BF_PREP2(x, name)        ( (x) & name##_MASK )
#define BF_GET2(y, name)         ( (y) & name##_MASK )
#define BF_SET2(y, x, name)      (y = ((y)&~name##_MASK) | BF_PREP2(x, name) )

#endif