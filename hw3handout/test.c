#include <stdio.h>
#include <stdlib.h>
#include "float_i2f.h"

typedef union {
  unsigned u;
  float f;
} uf_union_type;

float u2f(unsigned u) {
  uf_union_type a;
  a.u = u;
  return a.f;
}

unsigned f2u(float f) {
  uf_union_type a;
  a.f = f;
  return a.u;
}

int testOnei2f(int i, int detail) {
  float_bits f = float_i2f(i);
  float_bits answer = f2u((float)i);
  if (detail) {
    printf("i = %s0x%.8X, answer = 0x%.8X, your solution = 0x%.8X\n", (i<0)?"-":"", (i<0)?-i:i, answer, f);
    printf("answer:\nsign = %d, exp = 0x%X, frac = 0x%X\n", (answer>>31) & 1, (answer>>23) & 0xff, answer&0x7fffff);
  }

  if (f != answer) {
    if (detail) {
      printf("f:\nsign = %d, exp = 0x%X, frac = 0x%X\n", (f>>31) & 1, (f>>23) & 0xff, f&0x7fffff);
    }
    else {
      printf("i = %s0x%.8X, answer = 0x%.8X, your solution = 0x%.8X\n", (i<0)?"-":"", (i<0)?-i:i, answer, f);
    }

    return 0;
  }
  else {
    return 1; 
  }
}

void testAlli2f() {
  int i=0xFFFFFFFF;
  int count = 0;
  do {
    i = i + 1;
    // print out status for every 2^28 numbers tested
    if ((i&0x0FFFFFFF) == 0) {
      printf("Testing numbers between 0x%.8X to 0x%.8X\n", i,i+0x0FFFFFFF);
    }
    if (!testOnei2f(i, 0)) {
      count ++;
      if (count >= 10) {
        printf("Failed 10 tests. Aborting test.\n");
        break;
      }
    }
  } while (i != 0xffffffff);
  if (count == 0) {
    printf("Passed all tests for i2f.\n");
  }
}

int main(int argc, char **argv) {
  long input;
  if (argc > 1) {
    //testing a given integer
    input = strtol(argv[1],NULL,0);
    if(!testOnei2f((int)input,1)) {
      printf("Failed one test.\n");
    }
    else {
      printf("Passed one test for number %d.\n", (int)input);
    }
  }
  else {
    testAlli2f();
  }
  return 0;
}



