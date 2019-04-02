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

static char *print_lut(FILE *file, char *name, fixed_t start, fixed_t precision, fixed_t end, double (*func)(double)) {
  fprintf(file, "fixed_t %s[] = {\n", name);

  /*size_t table_size = FIXED_TO_INT(FIXED_DIV((end - start + 1), precision));
  fixed_t *table = malloc(sizeof(fixed_t) * table_size);*/

  for (fixed_t i = start; i <= end; i += precision) {
    int whole = i / FIXED_ONE;
    int part = i % FIXED_ONE;
    double f = whole + ((double) part / (double) FIXED_ONE);

    double r = func(f);

    int result = (int) (r * (double) FIXED_ONE);

    fprintf(file, "  %d,\n", result);
  }

  fprintf(file, "};\n");
}

int main(int argc, char **argv) {

  FILE *file = fopen("lut.h", "w");
  fprintf(file, "#include \"../logic.h\"\n");
  fprintf(file, "#define FIXED_SQRT(f) (FIXED_TO_INT(f) <= 1000 ? fixed_sqrt_table[FIXED_TO_INT(f)] : fixed_sqrt_table[FIXED_TO_INT(f)%%1000])\n");
  fprintf(file, "#define FIXED_SIN(f) (FIXED_TO_INT(f) <= 360 ? fixed_sin_table[FIXED_TO_INT(f)] : fixed_sin_table[FIXED_TO_INT(f)%%360])\n");
  fprintf(file, "#define FIXED_COS(f) (FIXED_TO_INT(f) <= 360 ? fixed_cos_table[FIXED_TO_INT(f)] : fixed_cos_table[FIXED_TO_INT(f)%%360])\n");
  print_lut(file, "fixed_sqrt_table", 0, FIXED_ONE, INT_TO_FIXED(1000), &sqrt);
  print_lut(file, "fixed_sin_table", 0, FIXED_ONE, INT_TO_FIXED(360), &sin);
  print_lut(file, "fixed_cos_table", 0, FIXED_ONE, INT_TO_FIXED(360), &cos);
  fclose(file);
}