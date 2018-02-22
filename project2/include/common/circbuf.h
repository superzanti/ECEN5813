/**
 * @file circbuf.h
 * @brief function and structure definitions for circular buffer
 *
 * this file contains the structure of a circular buffer, along with access
 * abstractions.  This circular buffer is meant to be utilized for a UART
 * communication system over GPIO pins
 *
 * Should be interrupt safe by utilizing the  volatile  keyword, and other
 * safeguard techniques
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */

#ifndef __CIRCBUF_H__
#define __CIRCBUF_H__

#include <stdint.h> /* for std ints: uint8_t, uint32_t */
#include <stddef.h> /* for size_t */

/*
 * quickly change what the type of the buffer is
 */
#define BUFFER_TYPE uint8_t

/*
 * The KL25Z needs to specifically disable interrupts
 */
#ifdef KL25Z
#define START_CRITICAL() __enable_irq()
#define END_CRITICAL() __disable_irq()
#else
#define START_CRITICAL()
#define END_CRITICAL()
#endif

/*
 * A structure to implement a FIFO buffer
 */
typedef struct {
    volatile BUFFER_TYPE* base;             // The base address of the buffer
    volatile size_t buff_size;              // the total size of the buffer
    volatile size_t head;                   // Where next byte should be put in the buff
    volatile size_t tail;                   // Oldest byte in buff
    volatile size_t num_in;                 // Number of bytes in buff
    volatile uint8_t buff_empty_flag:1;     // Flag to show if the buffer is empty
    volatile uint8_t buff_full_flag:1;      // Flag to show if the buffer is full
    volatile uint8_t buff_ovf_flag:1;       // Flag to show that the buffer has overflowed
    volatile uint8_t buff_destroyed_flag:1; // Flag to show that the buffer has been freed
} CB_t;


/*
 * An enumerator to allow for readability of errors
 */
typedef enum {
    SUCCESS,
    BAD_POINTER,
    NO_LENGTH,
    POSITION_TOO_LARGE,
    DOUBLE_FREE,
    NO_BUFFER_IN_MEMORY,
    FULL,
    EMPTY,
    CRITICAL_ERROR
} CB_e;

/*
 * An enumerator to set and unset flags
 */
typedef enum {
    UNSET = 0,
    SET = 1
} CB_f;

/* @brief initializes the circular buffer with default values
 *
 * the circularbuffer must already be allowcated when it is
 * passed in. The function will then create dynamic memory
 * to allocate the entire buffer. It will then set the
 * head, tail, and other values to be what they should be.
 * destroy must be called before free-ing the CB that
 * is passed in, otherwise a stack overflow may occur.
 *
 *
 * @param[in] CB_t the CB_t object to initialize
 * @param[in] size_t the size of the buffer to initialize
 * @return CB_e This function returns the CB_e typedef to indicate errors
 */
CB_e CB_init(CB_t circbuff, size_t buffer_size);

/* @brief frees the entire circular buffer
 *
 * this function simply frees the dynamic memory that it allowcated
 * when initializing the function.
 *
 * @param[in] CB_t the CB_t object to destroy
 * @return CB_e This function returns the CB_e typedef to indicate errors
 */
CB_e CB_destroy(CB_t circbuff);

/* @brief adds an item to the circular buffer
 *
 * add an item in memory tot he circular buffer
 * increment the head, and update all the flags
 *
 *
 * @param[in] CB_t the CB_t object to operate on
 * @param[in] BUFFER_TYPE the object to add into the buffer
 * @return CB_e This function returns the CB_e typedef to indicate errors
 */
CB_e CB_buffer_add_item(CB_t circbuff, BUFFER_TYPE data);

/* @brief removes an item from the circular buffer
 *
 * removes an item from the circular buffer by
 * incrementing the tail an decrementing the num_in
 *
 * @param[in] CB_t the CB_t object to operate on
 * @param[out] BUFFER_TYPE* put the data removed into this pointer
 * @return CB_e This function returns the CB_e typedef to indicate errors
 */
CB_e CB_buffer_remove_item(CB_t circbuff, BUFFER_TYPE* data);

/* @brief checks to see if the buffer is full
 *
 * simply checks the full flag of the buffer to see
 * if the head ever passed the tail
 *
 * @param[in] CB_t the CB_t object to operate on
 * @return CB_e This function returns the CB_e typedef to indicate errors
 */
CB_e CB_is_full(CB_t circbuff);

/* @brief checks to see if the buffer is empty
 *
 * simply checks the empty flag of the buffer
 * to see if the head and tail are equal and
 * there is no data in the buffer
 *
 *
 * @param[in] CB_t the CB_t object to operate on
 * @return CB_e This function returns the CB_e typedef to indicate errors
 */
CB_e CB_is_empty(CB_t circbuff);

/* @brief returns the value in the buffer at a position back from the head
 *
 * peeking at the 0th value will just return the last value
 * that was put into the buffer.
 *
 * @param[in] CB_t the CB_t object to operate on
 * @param[in] size_t the index away from the head
 * @param[out] BUFFER_TYPE* put the data peeked at into this pointer
 * @return CB_e This function returns the CB_e typedef to indicate errors
 */
CB_e CB_peek(CB_t circbuff, size_t position, BUFFER_TYPE* data);

#endif /*__CIRCBUF_H__*/
