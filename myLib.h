#ifndef MYLIB_H
#define MYLIB_H

// I don't really get what this file is supposed to contain lol
// I moved some of the stuff that's used in most places into this file but leaving some stuff in other headers where it makes sense

// --------------------- BIOS FUNCTIONS (INTERRUPTS) ---------------------
// Integer square root
extern int _isqrt(int i);

// --------------------- FIXED POINT MACROS & TYPES ---------------------
// Number of bits reserved for the fractional portion of the number (24.8, in this case)
#define FIXED_RESOLUTION 8

// Easy to convert back and forth with bit shifts by FIXED_RESOLUTION
// This works because we're just changing what the last FIXED_RESOLUTION bits represent
#define FIXED_TO_INT(F) ((F) >> FIXED_RESOLUTION)
#define INT_TO_FIXED(I) ((I) << FIXED_RESOLUTION)

// Multiplication in fixed point basically doubles the amount of fractional bits, so we can just truncate those
#define FIXED_MULT(A, B) (((A) * (B)) >> FIXED_RESOLUTION)
// Division in fixed point needs extra fractional bits on the numerator because the division truncates them
#define FIXED_DIV(A, B) (((fixed_t) (A) * (1 << FIXED_RESOLUTION)) / (B))

// Sqrt is similar to division here, we need to add extra bits before calculating it
#define FIXED_SQRT(F) (_isqrt((F) * (1 << FIXED_RESOLUTION)))

// Constant for 1 in fixed point
#define FIXED_ONE INT_TO_FIXED(1)

// Type defined to show the reader that a number is fixed point, but it's just a (32-bit + signed) int
typedef int fixed_t;

// --------------------- MISCELLANEOUS MACROS ---------------------
// Minimum/maximum of 2 numbers
#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x < y) ? y : x)

// --------------------- BIT FIELD SET/GET MACROS ---------------------
// These are used by a ton of the routines on sprites to set attributes quickly
// Basically they're just different bit shifts to prepend/get a field/set a field
#define BF_PREP(x, name)         ( ((x)<<name##_SHIFT)& name##_MASK  )
#define BF_GET(x, name)          ( ((x) & name##_MASK)>> name##_SHIFT )
#define BF_SET(y, x, name)       (y = ((y)&~name##_MASK) | BF_PREP(x,name) )

#define BF_PREP2(x, name)        ( (x) & name##_MASK )
#define BF_GET2(y, name)         ( (y) & name##_MASK )
#define BF_SET2(y, x, name)      (y = ((y)&~name##_MASK) | BF_PREP2(x, name) )

#endif