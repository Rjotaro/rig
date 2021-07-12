#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#ifdef DEBUG \

#define PRINT_D(X, ...) \
do \
  { \
    fprintf (stdout, __VA_ARGS__" %d\n", X); \
  } \
while (0) \
						
#define PRINT_F(X, ...) \
do \
	{ \
  	fprintf (stdout, __VA_ARGS__" %f\n", X); \
	} \
while (0)
						
#define PRINT_S(X) \
do \
  { \
    fprintf (stdout, "%s\n", X); \
  } \
while (0)

#else

#define PRINT_D(X, ...) do {} while (0)
						
#define PRINT_F(X, ...) do {} while (0)
						
#define PRINT_S(X) do {} while (0)

#endif

#endif /* __DEBUG_H__ */
