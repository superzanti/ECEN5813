/**
 * @file unittest.h
 * @brief function definitions for cmocka testing
 *
 * this file contains the necessary function definitions for
 * the unit testing using cmocka
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */

#ifndef __UNITTEST_H__
#define __UNITTEST_H__

/**
 * @file unittest.c
 * @brief implementation of unittest.h
 *
 * this file implements unittest.h,
 * testing an array of different functinos used
 * in this project
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */

/* test different lengths, over 256 could cause errors
 * since we're using uint8_ts and int8_ts
 */
#define TEST_LENGTH (256)

/* @brief test the functions in memory.h
 *
 * @param[in] a double void pointer to the state of the tests
 * @return void
 */
void memory_test(void **state);

/* @brief test the functions in conversion.h
 *
 * memmove tests
 * - Invalid Pointers - Should return fail if pointers are NULL
 * - No Overlap - Should return a pass for a move
 * - SRC in DST region Overlap - Should succeed at this
 * - DST in SRC region Overlap - Should succeed at this
 * - DST == SRC - Should be successful and likely skip the add
 * memset
 * - Invalid Pointers - Should return fail if pointers are NULL
 * - Check Set - Should accurately set region for length Value
 * memzero
 * - Invalid Pointers - Should return fail if pointers are NULL
 * - Check Set - Should accurately set region to zeroes
 * reverse
 * - Invalid Pointers - Should return fail if pointers are NULL
 * - Check Odd reverse - Should check that reverse succeeded for even length
 * - Check Even reverse - Should check that reverse succeeded for even length
 * - Check characters - Should be able to reverse any character set (256 byte array of 0-255)
 *
 * @param[in] a double void pointer to the state of the tests
 * @return void
 */
void conversion_test(void **state);

/* @brief test the functions in data.h
 *
 * atoi
 * - Invalid Pointers
 * - Zero Integer
 * - Max sized Integer
 * itoa
 * - Invalid Pointers
 * - Zero Integer
 * - Max sized Integer
 *
 * @param[in] a double void pointer to the state of the tests
 * @return void
 */
void data_test(void **state);

/* @brief test the functions in circbuf.h
 *
 * Endianness conversion
 * - Invalid Pointer
 * - Valid Conversion - Test that a big-to-little and little-to-big conversion worked
 * - Valid Conversion - Test that a big-to-little and little-to-bit produce the same as the
 *      original
 *
 * @param[in] a double void pointer to the state of the tests
 * @return void
 */
void circbuf_test(void **state);

/* @brief the caller function for the others
 *
 * implements cmocka to call a unittest of
 * several other tests
 *
 * @param[in] a double void pointer to the state of the tests
 * @return void
 */
int unittest(void);

#endif
