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
#include "cmocka.h"
#include "memory.h"
#include "conversion.h"
#include "data.h"
#include "circbuf.h"

#define TEST_LENGTH (256)

static void memory_test(void **state)
{
    size_t length = TEST_LENGTH;
    uint8_t * src = (uint8_t *)reserve_words(length);
    uint8_t * dst = (uint8_t *)reserve_words(length);

    /* send null src pointer and assert that null is returned
     * and that src and dst are still null for a memmove*/
    my_memmove((void*)NULL, dst, length);

    /* send null dst pointer and assert that null is returned
     * and that src and dst are still null for a memmove*/


    /* send null pointers and assert that null is returned
     * and that src and dst are still null for a memmove*/

    /* with src and dst in different places assert that
     * the arrays are equal after a memmove  */

    /* with dst and src pointing tot he same location
     * assert that the array is the same after a memmove */

    /* with dst starting at the middle of src
     * assert that values are correct after memmove */

    /* with src starting at the middle of dst
     * asset that values are correct after memmove */

    /* send null for src pointer and assert
     * that null is returned  for a memset*/

    /* send src and a value, assert that all elements
     * of the array are that value for a memset */

    /* send null for src pointer and assert
     * that null is returned  for a memzero*/

    /* send src assert that all elements
     * of the array are 0 for a memzero */

    /* send null pointer to reverse and assert
     * that a null pointer is returned */

    /* run mem reverse on a known array of odd length
     * assert that the returned array is reversed */

    /* run mem reverse on a known array of even length
     * assert that the returned array is reversed */

    /* create an array with TEST_LENGTH different
     * values and check that reversing it works */
}

static void conversion_test(void **state)
{
    /* send null pointer to atoi and assert that
     * a null pointer is returned */

    /* test sending max sized integer to atoi
     * and assert that the returned value is correct */

    /* test sending 0 to atoi and assert that
     * the returned value is correct */

    /* send null pointer to itoi and assert that
     * a null pointer is returned */

    /* test sending max sized integer to itoa
     * and assert that the returned value is correct */

    /* test sending 0 to itoa and assert that
     * the returned value is correct */
}

static void data_test(void **state)
{
    /* send a null pointer to endiannes conversion
     * asset that a null pointer is returned */

    /* test that a big endian to little endian
     * conversion works */

    /* test that a little endian to big endian
     * conversion works */
}

static void circbuf_test(void **state)
{
    /* snd a null pointer to each of the functions
     * and assert that a null pointer is returned */

    /* initialize a circular buffer and
     * assert that a buffer has been created */

    /* add then immediately remvoe and
     * assert that the same item was returned */

    /* fill the buffer and assert that itf
     * reports it is full, this also tests if the
     * adding can wrap around properly */

    /* add another item and assert that it
     * fails because the buffer is full. Also
     * assert that the overfil flag is set */

    /* empty the buffer and assert that
     * it reports that the buffer is empty,
     * this also tests if removing can wrap around
     * properly */

    /* remove one more item and assert that the buffer
     * fails to remove the item because there
     * are no items in the buffer */


}

int unittest(void)
{
    /* TODO create these functions in the header files  */
    const struct CMUnitTest tests[] =
    {
		cmocka_unit_test(memory_test),
		cmocka_unit_test(conversion_test),
		cmocka_unit_test(data_test),
		cmocka_unit_test(circbuf_test),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}



