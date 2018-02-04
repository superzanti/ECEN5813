#ifndef __DATA_H__
#define __DATA_H__

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
#endif /* __DATA_H__
