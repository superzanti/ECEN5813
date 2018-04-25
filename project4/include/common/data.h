/**
 * @file data.h
 * @brief functions to examine platform specific data type behavior
 *
 * contains functions for printing system specific type sizes, as well as
 * manually determining endianness, and swapping endianness on a given piece
 * of data, rendered as a byte array
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2018
 *
 */
#ifndef __DATA_H__
#define __DATA_H__

/* Type definitions needed for function prototypes */
#include <stdint.h>
#include <stddef.h>

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1
#define SWAP_NO_ERROR 0
#define SWAP_ERROR -1

/**
 * @brief function prints the size of c standard types
 *
 * this function calls sizeof, and stores the values in a temporary size_t
 * the size_t is reused for each call to sizeof
 * utilizes platform independent PRINTF macro to print information on variable
 * type and size
 * reports on
 *      char, short, int, long, double, float,
 *      unsigned char, unsigned int, unsigned long,
 *      signed char, signed int, signed long
 *
 * @param this takes in no parameters
 * @return void returns nothing
 */
void print_cstd_type_sizes();

/**
 * @brief this function prints the stdint type sizes
 *
 * this function calls sizeof, and stores the values in a temporary size_t
 * the size_t is reused for each call to sizeof
 * utilizes platform independent PRINTF macro to print information on variable
 * type and size
 * reports on
 *      int8_t, uint8_t, uint16_t, int32_t, uint32_t
 *      uint_fast8_t, uint_fast16_t, uint_fast32_t
 *      uint_least8_t, uint_least16_t, uint_least32_t
 *      size_t, ptrdiff_t
 *
 * @param this takes in no parameters
 * @return void returns nothing
 */
void print_stdint_type_sizes();

/**
 * @brief this function prints pointer sizes for a variety of types
 *
 * this function calls sizeof, and stores the values in a temporary size_t
 * the size_t is reused for each call to sizeof
 * utilizes platform independent PRINTF macro to print information on variable
 * type and size
 * reports on
 *      char *, short *, int *, double *, float *, void *,
 *      int8_t *, int16_t *, int32_t *,
 *      char **, int **, void **
 *
 * @param this takes in no parameters
 * @return void returns nothing
 */
void print_pointer_sizes();


/**
 * @brief this function reverses the endianness of a datatype
 *
 * this function uses my_reverse to swap a piece of data's bytewise endianness
 * it then returns a success or failure code accordingly
 *
 * returns either SWAP_NO_ERROR or SWAP_ERROR
 *
 * @param data is a pointer to the first byte of the data input
 * @param type_length holds the length of the data as with sizeof(data)
 * @return int32_t a macro return code signifying swap success or failure;
 */
int32_t swap_data_endianness(uint8_t * data, size_t type_length);

/**
 * @brief this function determines the endianness of the system
 *
 * this function manually creates a multibyte varible, then determines
 * if it is stored in big or little endian format
 *
 * @param this function takes in no parameters
 * @return uint32_t returns BIG_ENDIAN=1 or LITTLE_ENDIAN=0
 */
uint32_t determine_endianness();
#endif /* __DATA_H__*/
