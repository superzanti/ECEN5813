/**
 * @file logger_queue.h
 * @brief function and structure definitions for logger queue structure
 *
 * this file contains the structure of a circular buffer implementing a logging queue,
 * along with access abstractions.  This circular buffer is meant to be utilized for a UART
 * communication system over GPIO pins
 *
 * Should be interrupt safe by utilizing the  volatile  keyword, and other
 * safeguard techniques
 *
 * @author Seth Miers and Jake Cazden
 * @date April 29, 2018
 *
 * TODO should all size_ts be ints?
 *
 */

#ifndef __LOGGER_QUEUE_H__
#define __LOGGER_QUEUE_H__

#include <stdint.h> /* for std ints: uint8_t, uint32_t */
#include <stddef.h> /* for size_t */
#include "logger.h"

#ifdef KL25Z
#include "MKL25Z4.h"
#endif

/*
 * quickly change what the type of the buffer is
 */
#define BUFFER_TYPE log_t

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
 * TODO need to understand volatile should it be used here?
 */
typedef struct {
    volatile BUFFER_TYPE* base;             // The base address of the buffer
    volatile size_t buff_size;              // the total size of the buffer
    volatile BUFFER_TYPE* head;                   // Where next byte should be put in the buff
    volatile BUFFER_TYPE* tail;                   // Oldest byte in buff
    volatile size_t num_in;                 // Number of bytes in buff
    volatile uint8_t buff_empty_flag:1;     // Flag to show if the buffer is empty
    volatile uint8_t buff_full_flag:1;      // Flag to show if the buffer is full
    volatile uint8_t buff_ovf_flag:1;       // Flag to show that the buffer has overflowed
    volatile uint8_t buff_destroyed_flag:1; // Flag to show that the buffer has been freed
} LQ_t;


/*
 * An enumerator to allow for readability of errors
 */
typedef enum {
    LOGQUEUE_SUCCESS,
    LOGQUEUE_BAD_POINTER,
    LOGQUEUE_NO_LENGTH,
    LOGQUEUE_POSITION_TOO_LARGE,
    LOGQUEUE_DOUBLE_FREE,
    LOGQUEUE_NO_BUFFER_IN_MEMORY,
    LOGQUEUE_FULL,
    LOGQUEUE_EMPTY,
    LOGQUEUE_CRITICAL_ERROR
} LQ_e;

/*
 * An enumerator to set and unset flags
 */
typedef enum {
    UNSET = 0,
    SET = 1
} LQ_f;

/* @brief initializes the circular buffer with default values
 *
 * the circularbuffer must already be allowcated when it is
 * passed in. The function will then create dynamic memory
 * to allocate the entire buffer. It will then set the
 * head, tail, and other values to be what they should be.
 * destroy must be called before free-ing the LQ that
 * is passed in, otherwise a stack overflow may occur.
 *
 *
 * @param[in] LQ_t* the LQ_t object to initialize
 * @param[in] size_t the size of the buffer to initialize
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_init(LQ_t* circbuff, size_t buffer_size);

/* @brief frees the entire circular buffer
 *
 * this function simply frees the dynamic memory that it allowcated
 * when initializing the function.
 *
 * @param[in] LQ_t* the LQ_t object to destroy
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_destroy(LQ_t* circbuff);

/* @brief adds an item to the circular buffer
 *
 * add an item in memory tot he circular buffer
 * increment the head, and update all the flags
 *
 *
 * @param[in] LQ_t* the LQ_t object to operate on
 * @param[in] BUFFER_TYPE the object to add into the buffer
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_buffer_add_item(LQ_t* circbuff, BUFFER_TYPE data);

/* @brief removes an item from the circular buffer
 *
 * removes an item from the circular buffer by
 * incrementing the tail an decrementing the num_in
 *
 * @param[in] LQ_t* the LQ_t object to operate on
 * @param[out] BUFFER_TYPE* put the data removed into this pointer
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_buffer_remove_item(LQ_t* circbuff, BUFFER_TYPE* data);

/* @brief checks to see if the buffer is full
 *
 * simply checks the full flag of the buffer to see
 * if the head ever passed the tail
 *
 * @param[in] LQ_t# the LQ_t object to operate on
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_is_full(LQ_t* circbuff);

/* @brief checks to see if the buffer is empty
 *
 * simply checks the empty flag of the buffer
 * to see if the head and tail are equal and
 * there is no data in the buffer
 *
 *
 * @param[in] LQ_t* the LQ_t object to operate on
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_is_empty(LQ_t* circbuff);

/* @brief returns the value in the buffer at a position back from the head
 *
 * peeking at the 0th value will just return the last value
 * that was put into the buffer.
 *
 * @param[in] LQ_t* the LQ_t object to operate on
 * @param[in] size_t the index away from the head
 * @param[out] BUFFER_TYPE* put the data peeked at into this pointer
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_peek(LQ_t* circbuff, size_t position, BUFFER_TYPE* data);

#endif /*__LOGGER_QUEUE_H__*/
