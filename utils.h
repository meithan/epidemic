// Utility functions
#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>

// Returns a random integer in [a,b]
// Does not introduce bias, but may be slow if RAND_MAX % (b-a) is
// comparable to RAND_MAX (guaranteed to be fast if (b-a) is small compared
// to RAND_MAX).
int randint(int a, int b) {
  int max_int = RAND_MAX - (RAND_MAX % (b-a+1)) - 1;
  int r = rand();
  while (r > max_int) {
    r = rand();
  }
  return r % (b-a+1) + a;
}

// Returns a random integer in [0,n-1]
int randint(int n) {
  return randint(0,n-1);
}

// Returns a random real in [0,1)
double randreal() {
  return (double)rand()/(double)((unsigned)RAND_MAX+1);
}

#endif // UTILS_H
