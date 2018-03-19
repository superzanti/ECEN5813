/**
 * @file platform.h
 * @brief contains platform necessary includes and macros
 *
 * This contains the defines for the PRINTF macro and other functionality,
 * as well as ensuring the PRINTF macro doesn't cause errors
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2018
 *
 */
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/* defines to see if we have a PRINTF macro */
#if defined (__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#ifdef KL25Z
#define PRINTF(...)
#else
#include <stdio.h>
#define PRINTF(str, ...) printf(str, ##__VA_ARGS__)
#endif

#endif /* __PLATFORM_H__*/
