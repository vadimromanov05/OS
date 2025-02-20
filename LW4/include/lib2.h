#ifndef LIB2_H
#define LIB2_H

#define EXPORT
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
EXPORT double factorial(int n);
EXPORT char* translation(long x);
#ifdef __cplusplus
}
#endif

#endif