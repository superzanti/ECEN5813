#include "memory.h"

uint8_t * my_memmove(uint8_t * src, uint8_t * dst, size_t length)
{
    uint8_t my_array[length];
    my_memcpy(src, my_array, length);
    return my_memcpy(my_array, dst, length);
}

uint8_t * my_memcpy(uint8_t * src, uint8_t * dst, size_t length)
{
    size_t i = 0;
    uint8_t* orig_dst = dst;
    for(i=0;i<length;i++)
    {
        *dst++ = *src++;
    }
    return orig_dst;
}

uint8_t * my_memset(uint8_t * src, size_t length, uint8_t value)
{
    size_t i = 0;
    for(i=0;i<length;i++)
    {
        *src++ = value;
    }
    return src;
}

uint8_t * my_memzero(uint8_t * src, size_t length)
{
    return my_memset(src, length, 0); 
}

uint8_t * my_reverse(uint8_t * src, size_t length)
{
    uint8_t my_array[length];
    my_memcpy(src, my_array, length)
    my_array += length-1;
    for(i=0;i<length;i++)
    {
        *src++ = *my_array--;
    }
    return src;
}

void * reserve_words(size_t length)
{
}

uint8_t free_words(void * src)
{
}
