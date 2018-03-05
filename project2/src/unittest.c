/**
 * @file unittest.c
	status = CB_buffer_remove_item(my_cbuf, &data);
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

void memory_test(void **state)
{
    size_t length = TEST_LENGTH;
    uint8_t * src = (uint8_t *)reserve_words(length);
    uint8_t * dst = (uint8_t *)reserve_words(length);
    uint8_t * status;

    /* send null src pointer and assert that null is returned
     * and that src and dst are still null for a memmove*/
    status = my_memmove((void*)NULL, dst, length);
    assert_null(status);

    /* send null dst pointer and assert that null is returned
     * and that src and dst are still null for a memmove*/
    status = my_memmove(src, (void*)NULL, length);
    assert_null(status);

    /* send null pointers and assert that null is returned
     * and that src and dst are still null for a memmove*/
    status = my_memmove((void*)NULL, (void*)NULL, length);
    assert_null(status);

    /* with src and dst in different places assert that
     * the arrays are equal after a memmove  */
    for(uint16_t i = 0; i<(int)length; i++)
    {
    	*(src+i) = i;
    	*(dst+i) = (length - 1) - i;
    }
    status = my_memmove(src, dst, length);
    assert_non_null(status);
    for(uint16_t i = 0; i < length; i++)
    {
    	assert_int_equal(*(src+i), *(dst+i));
    }

    /* with dst and src pointing tot he same location
     * assert that the array is the same after a memmove */
    for(uint16_t i = 0; i<length; i++)
    {
    	*(src+i) = i;
    }
    status = my_memmove(src, src, length);
    assert_non_null(status);
    for(uint16_t i = 0; i < length; i++)
    {
    	assert_int_equal(*(src+i), i);
    }

    /* with dst starting at the middle of src
     * assert that values are correct after memmove */
    for(uint16_t i = 0; i<length; i++)
    {
    	*(src+i) = i;
    }
    status = my_memmove(src, src+length/3, length/2);
    assert_non_null(status);
    for(uint16_t i = 0; i < length/2; i++)
    {
    	assert_int_equal(*(src+length/3+i), i);
    }

    /* with src starting at the middle of dst
     * asset that values are correct after memmove */
    for(uint16_t i = 0; i<length/2; i++)
    {
    	*(src+length/3+i) = i;
    }
    status = my_memmove(src+length/3, src, length/2);
    assert_non_null(status);
    for(uint16_t i = 0; i < length/2; i++)
    {
    	assert_int_equal(*(src+i), i);
    }

    /* send null for src pointer and assert
     * that null is returned  for a memset*/
    status = my_memset((void*)NULL, length, 200);
    assert_null(status);

    /* send src and a value, assert that all elements
     * of the array are that value for a memset */
    status = my_memset(src, length, 200);
    assert_non_null(status);
    for(uint16_t i = 0; i < length; i++)
    {
    	assert_int_equal(*(src+i), 200);
    }

    /* send null for src pointer and assert
     * that null is returned  for a memzero*/
    status = my_memzero((void*)NULL, length);
    assert_null(status);

    /* send src assert that all elements
     * of the array are 0 for a memzero */
    status = my_memzero(src, length);
    assert_non_null(status);
    for(uint16_t i = 0; i < length; i++)
    {
    	assert_int_equal(*(src+i), 0);
    }

    /* send null pointer to reverse and assert
     * that a null pointer is returned */
    status = my_reverse((void*)NULL, length);
    assert_null(status);

    /* run mem reverse on a known array of odd length
     * assert that the returned array is reversed */
    for(uint16_t i = 0; i<length; i++)
    {
    	*(src+i) = i;
    }
    status = my_reverse(src, length);
    assert_non_null(status);
    for(uint16_t i = 0; i < length; i++)
    {
    	assert_int_equal(*(src+i), (length-1)-i);
    }

    /* run mem reverse on a known array of even length
     * assert that the returned array is reversed */
    for(uint16_t i = 0; i<length-1; i++)
    {
    	*(src+i) = i;
    }
    status = my_reverse(src, length-1);
    assert_non_null(status);
    for(uint16_t i = 0; i < length-1; i++)
    {
    	assert_int_equal(*(src+i), (length-2)-i);
    }
}

void conversion_test(void **state)
{
	uint8_t ptr_max[10] = "2147483647";
	uint8_t ptr_min[11] = "-2147483647";
	uint8_t ptr_min2[11] = "-2147483647";
	uint32_t statusi = 0;
	uint8_t statusa[10] = "0000000000";
	uint8_t statusl = 0;
    /* send null pointer to atoi and assert that
     * a null pointer is returned */
	statusi = my_atoi((void*)NULL, 3, 10);
	assert_int_equal(statusi, 0);

    /* test sending max sized integer to atoi
     * and assert that the returned value is correct */
	statusi = my_atoi(ptr_max, 10, 10);
	assert_int_equal(statusi, 2147483647);

    /* test sending 0 to atoi and assert that
     * the returned value is correct */
	statusi = my_atoi(ptr_min, 1, 10);
	assert_int_equal(statusi, 0);

    /* send null pointer to itoi and assert that
     * a null pointer is returned */
	statusl = my_itoa(2147483647, (void*)NULL, 10);
    assert_int_equal(statusl, 0);

    /* test sending max sized integer to itoa
     * and assert that the returned value is correct */
	statusl = my_itoa(2147483647, statusa, 10);
    assert_int_equal(statusl, 10);
    for(uint16_t i = 0; i<10; i++)
    {
    	assert_int_equal(*(statusa+i), *(ptr_max+i));
    }

    /* test sending min to itoa and assert that
     * the returned value is correct */
	statusl = my_itoa(-2147483647, statusa, 10);
    assert_int_equal(statusl, 11);
    for(uint16_t i = 0; i<1; i++)
    {
    	assert_int_equal(*(statusa+i), *(ptr_min2+i));
    }
}

void data_test(void **state)
{
	int32_t status = 0;
	uint8_t data_orig[4] = {1, 2, 4, 8};
	uint8_t data[4] = {1, 2, 4, 8};
	size_t type_length = 4; /* bytes in a uint32 */
    /* send a null pointer to endiannes conversion
     * asset that a null pointer is returned */
	status = swap_data_endianness((void*)NULL, type_length);
	assert_int_equal(status,SWAP_ERROR);

    /* test that a big endian to little endian
     * conversion works */
	status = swap_data_endianness(data, type_length);
	assert_int_equal(status,SWAP_NO_ERROR);
	for(int i = 0; i<type_length; i++)
	{
		assert_int_equal(*(data+i), *(data_orig+(type_length-1)-i));
	}
	/* undo swap */
	status = swap_data_endianness(data, type_length);
	assert_int_equal(status,SWAP_NO_ERROR);

    /* test that a little endian to big endian
     * conversion works */
	status = swap_data_endianness(data, type_length);
	assert_int_equal(status,SWAP_NO_ERROR);
	for(int i = 0; i<type_length; i++)
	{
		assert_int_equal(*(data+i), *(data_orig+(type_length-1)-i));
	}
}

void circbuf_test(void **state)
{
	CB_t *my_cbuf = malloc(sizeof(CB_t));
    size_t length = TEST_LENGTH;
    uint8_t data = '0';
    CB_e status = SUCCESS;
    /* snd a null pointer to each of the functions
     * and assert that a null pointer is returned */
	status = CB_init((void*)NULL, length);
	assert_int_equal(status, BAD_POINTER);

    /* initialize a circular buffer and
     * assert that a buffer has been created */
	status = CB_init(my_cbuf, length);
	assert_int_equal(status, SUCCESS);

    /* add then immediately remvoe and
     * assert that the same item was returned */
	status = CB_buffer_add_item(my_cbuf, 'S');
	assert_int_equal(status, SUCCESS);
	assert_int_equal(status, SUCCESS);
	status = CB_buffer_remove_item(my_cbuf, &data);
	assert_int_equal(status, SUCCESS);
	assert_int_equal(data, 'S');

    /* fill the buffer and assert that itf
     * reports it is full, this also tests if the
     * adding can wrap around properly */
	for(int i = 0; i<length; i++)
	{
		status = CB_buffer_add_item(my_cbuf, i);
		assert_int_equal(status, SUCCESS);
	}

    /* add another item and assert that it
     * fails because the buffer is full. Also
     * assert that the overfil flag is set */
	status = CB_buffer_add_item(my_cbuf, 'S');
	assert_int_equal(status, FULL);
	assert_int_equal(my_cbuf->buff_full_flag, SET);

    /* empty the buffer and assert that
     * it reports that the buffer is empty,
     * this also tests if removing can wrap around
     * properly */
	for(int i = 0; i<length; i++)
	{
		status = CB_buffer_remove_item(my_cbuf, &data);
		assert_int_equal(status, SUCCESS);
		assert_int_equal(data, i);
	}

    /* remove one more item and assert that the buffer
     * fails to remove the item because there
     * are no items in the buffer */
	status = CB_buffer_remove_item(my_cbuf, &data);
	assert_int_equal(status, EMPTY);

}

int unittest()
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



