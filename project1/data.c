/**
 * @file data.h
 * @brief implementation of data.h
 *
 * @author Jake Cazden
 * @date February 4, 2017
 *
 */
#include "data.h"
#include "memory.h"
#include "platform.h"

#if defined (__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
void print_cstd_type_sizes()
{/*this function is pretty straightforward, it prints a list of types and
   their sizes*/
    size_t temp=sizeof(char);
    PRINTF("char:           %d\n",temp);
    temp=sizeof(short);
    PRINTF("short:          %d\n",temp);
    temp=sizeof(int);
    PRINTF("int:            %d\n",temp);
    temp=sizeof(long);
    PRINTF("long:           %d\n",temp);
    temp=sizeof(double);
    PRINTF("double:         %d\n",temp);
    temp=sizeof(float);
    PRINTF("float:          %d\n",temp);
    temp=sizeof(unsigned char);
    PRINTF("unsigned char:  %d\n",temp);
    temp=sizeof(unsigned int);
    PRINTF("unsigned int:   %d\n",temp);
    temp=sizeof(unsigned long);
    PRINTF("unsigned long:  %d\n",temp);
    temp=sizeof(signed char);
    PRINTF("signed char:    %d\n",temp);
    temp=sizeof(signed int);
    PRINTF("signed int:     %d\n",temp);
    temp=sizeof(signed long);
    PRINTF("signed long:    %d\n",temp);
    return;
}
void print_stdint_type_sizes()
{/* this function is pretty straightforward, it prints a list of stdint types
    and their sizes*/
    size_t temp=sizeof(int8_t);
    PRINTF("int8_t:         %d\n",temp);
    temp=sizeof(uint8_t);
    PRINTF("uint8_t:        %d\n",temp);
    temp=sizeof(uint16_t);
    PRINTF("uint16_t:       %d\n",temp);
    temp=sizeof(int32_t);
    PRINTF("int32_t:        %d\n",temp);
    temp=sizeof(uint32_t);
    PRINTF("uint32_t:       %d\n",temp);
    temp=sizeof(uint_fast8_t);
    PRINTF("uint_fast8_t:   %d\n",temp);
    temp=sizeof(uint_fast16_t);
    PRINTF("uint_fast16_t:  %d\n",temp);
    temp=sizeof(uint_fast32_t);
    PRINTF("uint_fast32_t:  %d\n",temp);
    temp=sizeof(uint_least8_t);
    PRINTF("uint_least8_t:  %d\n",temp);
    temp=sizeof(uint_least16_t);
    PRINTF("uint_least16_t: %d\n",temp);
    temp=sizeof(uint_least32_t);
    PRINTF("uint_least32_t: %d\n",temp);
    temp=sizeof(size_t);
    PRINTF("size_t:         %d\n",temp);
    temp=sizeof(ptrdiff_t);
    PRINTF("ptrdiff_t:      %d\n",temp);
    return;
}
void print_pointer_sizes()
{
    size_t temp=sizeof(char*);
    PRINTF("char*:          %d\n",temp);
    temp=sizeof(short*);
    PRINTF("short*:         %d\n",temp);
    temp=sizeof(int*);
    PRINTF("int*:           %d\n",temp);
    temp=sizeof(double*);
    PRINTF("double*:        %d\n",temp);
    temp=sizeof(float*);
    PRINTF("float*:         %d\n",temp);
    temp=sizeof(void*);
    PRINTF("void*:          %d\n",temp);
    temp=sizeof(int8_t*);
    PRINTF("int8_t*:        %d\n",temp);
    temp=sizeof(int16_t*);
    PRINTF("int16_t*:       %d\n",temp);
    temp=sizeof(int32_t*);
    PRINTF("int32_t*:       %d\n",temp);
    temp=sizeof(char**);
    PRINTF("char**:         %d\n",temp);
    temp=sizeof(int**);
    PRINTF("int**:          %d\n",temp);
    temp=sizeof(void**);
    PRINTF("void**:         %d\n",temp);
    return;
}
uint32_t swap_data_endianness(uint8_t * data, size_t type_length)
{/*TODO make this funcction nondestructive on failure if possible*/
    /* TODO possible bad return type because of function being unsigned */
    if(!data) return SWAP_ERROR;/*if null, return error*/
    uint8_t test[type_length];/*block out a region on the stack*/
    my_memmove(data,test,type_length);/*create a copy of 'data' on the stack*/
    my_reverse(data , type_length);/*reverse data*/
    uint32_t i;
    for(i=0;i<(type_length/2);i++)
    {
        if(*(test+i)!=*(data+type_length-1-i))
        {
            return SWAP_ERROR; /* TODO possible bad return type because of function being unsigned */
        }
    }
    return SWAP_NO_ERROR;
}
uint32_t determine_endianness()
{
    uint32_t test = 0x00BADA55;/*standard test word*/
    uint8_t* ptr = (uint8_t*)&test;
    if(*(ptr)==5)
    {
        return LITTLE_ENDIAN;
    }
    else
    {
        return BIG_ENDIAN;
    }
}
