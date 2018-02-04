#include "data.h"
#include "memory.h"
#include <stdint.h>

#define __PRINTF_DEBUG__\
        #if defined (__GNUC__)\
        #pragma GCC diagnostic ignored "-Wunused-but-set-variable"\
        #endif
void print_cstd_type_sizes()
{
        size_t temp;
        temp=sizeof(char);
        __PRINTF_DEBUG__
        PRINTF("char:           %d\n",temp);
        temp=sizeof(short);
        __PRINTF_DEBUG__
        PRINTF("short:          %d\n",temp);
        temp=sizeof(int);
        __PRINTF_DEBUG__
        PRINTF("int:            %d\n",temp);
        temp=sizeof(long);
        __PRINTF_DEBUG__
        PRINTF("long:           %d\n",temp);
        temp=sizeof(double);
        __PRINTF_DEBUG__
        PRINTF("double:         %d\n",temp);
        temp=sizeof(float);
        __PRINTF_DEBUG__
        PRINTF("float:          %d\n",temp);
        temp=sizeof(unsigned char);
        __PRINTF_DEBUG__
        PRINTF("unsigned char:  %d\n",temp);
        temp=sizeof(unsigned int);
        __PRINTF_DEBUG__
        PRINTF("unsigned int:   %d\n",temp);
        temp=sizeof(unsigned long);
        __PRINTF_DEBUG__
        PRINTF("unsigned long:  %d\n",temp);
        temp=sizeof(signed char);
        __PRINTF_DEBUG__
        PRINTF("signed char:    %d\n",temp);
        temp=sizeof(signed int);
        __PRINTF_DEBUG__
        PRINTF("signed int:     %d\n",temp);
        temp=sizeof(signed long);
        __PRINTF_DEBUG__
        PRINTF("signed long:    %d\n",temp);
        return;
}
void print_stdint_type_sizes()
{
        size_t temp;
        temp=sizeof(int8_t);
        __PRINTF_DEBUG__
        PRINTF("int8_t:         %d\n",temp);
        temp=sizeof(uint8_t);
        __PRINTF_DEBUG__
        PRINTF("uint8_t:        %d\n",temp);
        temp=sizeof(uint16_t);
        __PRINTF_DEBUG__
        PRINTF("uint16_t:       %d\n",temp);
        temp=sizeof(int32_t);
        __PRINTF_DEBUG__
        PRINTF("int32_t:        %d\n",temp);
        temp=sizeof(uint32_t);
        __PRINTF_DEBUG__
        PRINTF("uint32_t:       %d\n",temp);
        temp=sizeof(uint_fast8_t);
        __PRINTF_DEBUG__
        PRINTF("uint_fast8_t:   %d\n",temp);
        temp=sizeof(uint_fast16_t);
        __PRINTF_DEBUG__
        PRINTF("uint_fast16_t:  %d\n",temp);
        temp=sizeof(uint_fast32_t);
        __PRINTF_DEBUG__
        PRINTF("uint_fast32_t:  %d\n",temp);
        temp=sizeof(uint_least8_t);
        __PRINTF_DEBUG__
        PRINTF("uint_least8_t:  %d\n",temp);
        temp=sizeof(uint_least16_t);
        __PRINTF_DEBUG__
        PRINTF("uint_least16_t: %d\n",temp);
        temp=sizeof(uint_least32_t);
        __PRINTF_DEBUG__
        PRINTF("uint_least32_t: %d\n",temp);
        temp=sizeof(size_t);
        __PRINTF_DEBUG__
        PRINTF("size_t:         %d\n",temp);
        temp=sizeof(ptrdiff_t);
        __PRINTF_DEBUG__
        PRINTF("ptrdiff_t:      %d\n",temp);
        return;
}
void print_pointer_sizes()
{
        size_t temp;
        temp=sizeof(char*);
        __PRINTF_DEBUG__
        PRINTF("char*:          %d\n",temp);
        temp=sizeof(short*);
        __PRINTF_DEBUG__
        PRINTF("short*:         %d\n",temp);
        temp=sizeof(int*);
        __PRINTF_DEBUG__
        PRINTF("int*:           %d\n",temp);
        temp=sizeof(double*);
        __PRINTF_DEBUG__
        PRINTF("double*:        %d\n",temp);
        temp=sizeof(float*);
        __PRINTF_DEBUG__
        PRINTF("float*:         %d\n",temp);
        temp=sizeof(void*);
        __PRINTF_DEBUG__
        PRINTF("void*:          %d\n",temp);
        temp=sizeof(int8_t*);
        __PRINTF_DEBUG__
        PRINTF("int8_t*:        %d\n",temp);
        temp=sizeof(int16_t*);
        __PRINTF_DEBUG__
        PRINTF("int16_t*:       %d\n",temp);
        temp=sizeof(int32_t*);
        __PRINTF_DEBUG__
        PRINTF("int32_t*:       %d\n",temp);
        temp=sizeof(char**);
        __PRINTF_DEBUG__
        PRINTF("char**:         %d\n",temp);
        temp=sizeof(int**);
        __PRINTF_DEBUG__
        PRINTF("int**:          %d\n",temp);
        temp=sizeof(void**);
        __PRINTF_DEBUG__
        PRINTF("void**:         %d\n",temp);
        return;
}
