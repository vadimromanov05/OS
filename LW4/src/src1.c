#include <math.h>

#include "../include/lib1.h"

// Ряд Лейбница
float Pi(const int K) {
  float pi = 0.0;
  int sign = 1;
  for (int i = 0; i < K; i++) {
    pi += sign * 4.0 / (2 * i + 1);
    sign = -sign;
  }
  return pi;
}

float E(const int x) {
  if (x == 0) {
    return 1;
  }
  return pow(1.0 + 1.0 / x, x);
}