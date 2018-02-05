#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/* defines to see if we have a PRINTF macro */
#if defined (__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

size_t temp;

temp = sizeof(char);
PRINTF("sizeof(char) = %d", temp);

temp = sizeof(short);
PRINTF("sizeof(short) = %d", temp);

temp = sizeof(int);
PRINTF("sizeof(int) = %d", temp);

temp = sizeof(long);
PRINTF("sizeof(long) = %d", temp);

temp = sizeof(double);
PRINTF("sizeof(double) = %d", temp);

temp = sizeof(float);
PRINTF("sizeof(float) = %d", temp);

temp = sizeof(void);
PRINTF("sizeof(void) = %d", temp);

#endif /* __PLATFORM_H__
