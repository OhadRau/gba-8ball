/*
This file generates lookup tables for common math operations in fixed point.

We generate the following functions:
- sqrt
- sin
- cos
*/

#include <stdio.h>
#include <math.h>
#include "../logic.h"

// So for some reason this works better than the normal sqrt here...
/*static fixed_t fixed_sqrt(fixed_t f) {
  if (f < INT_TO_FIXED(2)) {
    return f;
  }
  fixed_t next = 2 * fixed_sqrt(f >> 2);
  if (FIXED_MULT((next + FIXED_ONE), (next + FIXED_ONE)) > f) {
    return next;
  } else {
    return next + FIXED_ONE;
  }
}*/

static char *print_fixed_lut(FILE *file, char *name, fixed_t start, fixed_t precision, fixed_t end, fixed_t (*func)(fixed_t)) {
  fprintf(file, "fixed_t %s[] = {\n", name);

  for (fixed_t i = start; i <= end; i += precision) {
    fixed_t result = func(i);
    fprintf(file, "  %d,\n", result);
  }

  fprintf(file, "};\n");
}

static char *print_lut(FILE *file, char *name, fixed_t start, fixed_t precision, fixed_t end, double (*func)(double)) {
  fprintf(file, "fixed_t %s[] = {\n", name);

  for (fixed_t i = start; i <= end; i += precision) {
    int whole = i / FIXED_ONE;
    int part = i % FIXED_ONE;
    double f = whole + ((double) part / (double) FIXED_ONE);

    // Doesn't matter rn but don't want this if it's not a trig function getting generated
    double r = func(f * M_PI / 180.0f);

    int result = (int) ceil(r * (double) FIXED_ONE);

    fprintf(file, "  %d,\n", result);
  }

  fprintf(file, "};\n");
}

int main(int argc, char **argv) {

  FILE *file = fopen("lut.h", "w");
  fprintf(file, "#ifndef LUT_H\n#define LUT_H\n");
  fprintf(file, "#include \"../logic.h\"\n");
  //fprintf(file, "#define FIXED_SQRT_POS(f) ((f) <= (1000 * FIXED_ONE/4) ? fixed_sqrt_table[(f)] : fixed_sqrt_table[(f)%%(1000 * FIXED_ONE/4)])\n");
  //fprintf(file, "#define FIXED_SQRT(f) ((f) < 0 ? FIXED_SQRT_POS(-(f)) : FIXED_SQRT_POS(f))\n");
  fprintf(file, "#define FIXED_SIN(f) (FIXED_TO_INT(f) <= 360 ? fixed_sin_table[FIXED_TO_INT(f)] : fixed_sin_table[FIXED_TO_INT(f)%%360])\n");
  fprintf(file, "#define FIXED_COS(f) (FIXED_TO_INT(f) <= 360 ? fixed_cos_table[FIXED_TO_INT(f)] : fixed_cos_table[FIXED_TO_INT(f)%%360])\n");
  //print_lut(file, "fixed_sqrt_table", 0, 4, INT_TO_FIXED(200), &sqrt); // precision = 4/FIXED_ONE
  print_lut(file, "fixed_sin_table", 0, FIXED_ONE, INT_TO_FIXED(360), &sin);
  print_lut(file, "fixed_cos_table", 0, FIXED_ONE, INT_TO_FIXED(360), &cos);
  fprintf(file, "#endif");
  fclose(file);
}
