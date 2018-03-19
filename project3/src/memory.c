/**
 * @file memory.c
 * @brief implements memory.h
 *
 * implements software defined functions for copying, allocating, and freeing
 * memory, without relying on hardware specifically.
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2018
 *
 */
#include "memory.h"

/* Only need the following if we cant use
 * built in malloc and free functions
 * extern void __HeapBase, __HeapLimit;
 */

/* Needed to return null pointers
 * This is defined in stddef so it's commented out
 *
 *  #ifndef NULL
 *  #define NULL ((void*)0)
 *  #endif
 */

#include <stdlib.h>

uint8_t * memset_dma(uint8_t * src, size_t length, uint8_t value)
{
	/* TODO implement function */
	#ifdef KL25Z
	return src;
	#else
	return my_memmove(src, length, value);
	#endif
}

uint8_t * memmove_dma(uint8_t * src, uint8_t * dst, size_t length)\
{
	/* TODO implement function */
	#ifdef KL25Z
	return dst;
	#else
	return my_memmove(src, dst, length);
	#endif
}

uint8_t * my_memmove(uint8_t * src, uint8_t * dst, size_t length)
{
    /* check for null pointers */
    if(!dst)
    {
        return NULL;
    }
    if(!src)
    {
        return NULL;
    }
    /* create a new array to copy into to prevent corruption */
    uint8_t my_array[length];
    /* copy our current array into the new array */
    my_memcpy(src, my_array, length);
    /* copy the new array into the destination array*/
    return my_memcpy(my_array, dst, length);
}

uint8_t * my_memcpy(uint8_t * src, uint8_t * dst, size_t length)
{
    /* check for null pointers */
    if(!dst)
    {
        return NULL;
    }
    if(!src)
    {
        return NULL;
    }
    /* create a variable to iterate through (for loop) */
    size_t i = 0;
    /* save the original destination since we'll be changing its value */
    uint8_t* orig_dst = dst;
    /* loop for how many bytes there are */
    for(i=0;i<length;i++)
    {
        /* copy the source to the destination */
        *dst++ = *src++;
    }
    return orig_dst;
}

uint8_t * my_memset(uint8_t * src, size_t length, uint8_t value)
{
    /* check for null pointers */
    if(!src)
    {
        return NULL;
    }
    /* create a variable to iterate through (for loop) */
    size_t i = 0;
    /* loop for how many bytes there are */
    for(i=0;i<length;i++)
    {
        /* copy the value into the source */
        *src++ = value;
    }
    return src;
}

uint8_t * my_memzero(uint8_t * src, size_t length)
{
    /* just call my_memset with a value of 0 */
    return my_memset(src, length, 0); 
}

uint8_t * my_reverse(uint8_t * src, size_t length)
{
    /* check for null pointers */
    if(!src)
    {
        return NULL;
    }
    /* create a temporary array */
    uint8_t my_array[length];
    /* copy the source into the temporary array */
    my_memcpy(src, my_array, length);
    /* create an address that points to the end of the array */
    uint8_t * my_array_ptr = &my_array[length-1];
    /* loop for how many bytes there are */
    size_t i = 0;
    for(i=0;i<length;i++)
    {
        /* copy the destination of the pointer
         * of the end of the array into the srouce */
        *src++ = *my_array_ptr--;
    }
    return src;
}

void * reserve_words(size_t length)
{
    void* src;
    /* malloc will return null if it fails */
    src = (void*)malloc(length*sizeof(void*));
    return src;
}

uint8_t free_words(void * src)
{
    free(src);
    return 0;
}
