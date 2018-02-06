/*
 * @file conversion.c
 * @brief This file is to be used to project 1.
 *
 * @author Jake Cazden and Seth Miers
 * @date February 4, 2018
 *
 * */
#include<stdint.h>
#include"conversion.h"

/*TODO remove this*/
#include "platform.h"

int32_t exponent(int32_t base,int32_t power)
{
    if(power<0) return 0;
    uint32_t i=0;
    int32_t retval=1;
    for(i=1;i<=power;i++)
    {
        retval*=base;
    }
    return retval;
}

uint8_t my_itoa(int32_t data, uint8_t * ptr, uint32_t base)
{
    uint8_t length=0;
    if(base<BASE_2||base>BASE_16)
    {
        return 0;
    }
    if(data<0)
    {
        *ptr++='-';
        length++;
        data=0-data;/*make the number positive*/
    }
    uint32_t i;
    uint32_t remainder=0; /* TODO remainder never referenced? */
    for(i=0;i<32;i++)/*figure out the magnitude of the number*/
    {
        if(data==data%exponent(base,i))
        {
            break;
        }
    }
    int32_t j=i-1;
    uint8_t num;
    PRINTF("\n current value of data is: %d\n", data);
    for(j;j>=0;j--)
    {
        num=(data-data%exponent(base,j))/exponent(base,j);/*calculate the MSB*/
        data-=num*exponent(base,j);/*subtract off the MSB*/
    }
    if(num>9)
    {
        *ptr++=num+55;
        length++;
    }
    else
    {
        *ptr++=num+48;
        length++;
    }
    return length;
}


/**
 * @brief function to convert an ascii string into an integer
 * 
 * This function takes in an array of data (ptr) and
 * convertes it to the 4byte equivalant. It is assumed
 * that the array of ascii formatted bytes (string) that
 * is passed in is null '\0' terminated. However,
 * the number of digits in this string is also passed in.
 * The function can convert anything of base 2 to 16.
 * The first character of the string may be a '-' to
 * indicate that it is a negative number.
 *
 * The entire function is done using only
 * pointer arithmetic.
 *
 * @param ptr a pointer to the array of bytes that store ascii characters
 * @param digits the number of characters in the string
 * @param base is the base we want to convert from, bases 2-16 are supported
 * @return int8_t the converted number
 */
int32_t my_atoi(uint8_t * ptr, uint8_t digits, uint32_t base)
{
    uint8_t i = 0;
    uint8_t negative = 0;
    int32_t return_value;
    /* check to see if the integer is negative*/
    if(*ptr=='-')/* using int is okay here */
    {
        negative = 1;
    }
    /* loop through each digit and add it to the return_value*/
    for(i=negative;i<digits;i++)
    {
        /* the digit is between a 0 and a 9 */
        if (*ptr >= 48 && *ptr <= 57)
        {
            if((*ptr - 48) >= base)
                /* an error has occured, we've encountered an unsupported character */
                return 0;
            return_value = (*ptr++ - 48) * exponent(base, i);
        }
        /* the digit is between an uppercase A and a F */
        else if (*ptr >= 65 && *ptr <= 70)
        {
            if((*ptr - 55) >= base)
                /* an error has occured, we've encountered an unsupported character */
                return 0;
            return_value = (*ptr++ - 55) * exponent(base, i);
        }
        /* the digit is between a lowercase a and a f */
        else if (*ptr >= 97 && *ptr <= 102)
        {
            if((*ptr - 87) >= base)
                /* an error has occured, we've encountered an unsupported character */
                return 0;
            return_value = (*ptr++ - 87) * exponent(base, i);
        }
        else
        {
            /* an error has occurred, we've encountered an unsupported character */
            return 0;
        }
    }
    if(negative)
        return return_value*-1;
    return return_value;
}
