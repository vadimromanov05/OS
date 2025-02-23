#include <math.h>
#include "../include/lib1.h"


int GCF(int a, int b) {
  if(a < 0 || b < 0){
      printf("Numbers must be natural ir 0.\n");
      return 0;
  }
  
  int divider = 1;
  int min = 0;
  if(a == 0){ min = b; }
  else if(b == 0){ min = a; }
  else if(a < b){ min = a; }
  else {min = b; }
  for(int i=1; i<=min; ++i) {
      if((a % i == 0) && (b % i == 0)) {
          divider = i;
      }
  }
  return divider;
}


float E(const int x) {
  float e = 1.0;
  float fact = 1.0;

  for (int n = 1; n <= x; n++) {
      fact *= n;
      e += 1.0 / fact;
  }
  return e;
}