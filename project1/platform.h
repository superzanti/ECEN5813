#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/* defines to see if we have a PRINTF macro */
#if defined (__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

/* TODO we might not be allowed to use stddef but it's needed for size_t */
#include <stddef.h>

#include <stdio.h>
#define PRINTF(str, ...) printf(str, ##__VA_ARGS__)
#define print_array(x, y) ;

#endif /* __PLATFORM_H__*/
