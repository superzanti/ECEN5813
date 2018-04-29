/**
 * @file logger_queue.c
 * @brief implementation of logger_queue.h
 *
 * implements the logging queue circular buffer access functions found in logger_queue.h
 *
 * @author Seth Miers and Jake Cazden
 * @date April 29, 2018
 *
 */
#include "logger_queue.h"
#include "logger.h"
#include "circbuf.h"
#ifdef KL25Z
#include "uart.h"
#endif

/* standard library for malloc */
#include <stdlib.h>

/* @brief initializes the logger queue with default values
 *
 * the logger queue must already be allocated when it is
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
LQ_e LQ_init(LQ_t* logbuff, size_t buffer_size)
{
    if ((int)buffer_size == 0)
        return LOGQUEUE_NO_LENGTH;
    if ((void*)logbuff == NULL)
        return LOGQUEUE_BAD_POINTER;
    logbuff->buff_size = buffer_size;
    logbuff->base = (log_t**)malloc(((int)buffer_size)*sizeof(log_t*));
    logbuff->head = logbuff->base;
    logbuff->tail = logbuff->base;
    logbuff->num_in = 0;
    logbuff->buff_empty_flag = LOGGER_SET;
    logbuff->buff_full_flag = LOGGER_UNSET;
    logbuff->buff_ovf_flag = LOGGER_UNSET;
    logbuff->buff_destroyed_flag = LOGGER_UNSET;
    if ((void*)logbuff->base == NULL)
    {
        logbuff->buff_size = 0;
        logbuff->buff_full_flag = LOGGER_SET;
        logbuff->buff_destroyed_flag = LOGGER_SET;
        logbuff->buff_ovf_flag = LOGGER_SET;
        return LOGQUEUE_NO_BUFFER_IN_MEMORY;
    }
    return LOGQUEUE_SUCCESS;
}

/* @brief frees the entire logger queue
 *
 * this function simply frees the dynamic memory that it allowcated
 * when initializing the function.
 *
 * @param[in] LQ_t* the LQ_t object to destroy
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_destroy(LQ_t* logbuff)
{
    if ((void*)logbuff == NULL)
        return LOGQUEUE_BAD_POINTER;
    /* we don't want to free something twice */
    if ((LQ_f)logbuff->buff_destroyed_flag == LOGGER_SET)
        return LOGQUEUE_DOUBLE_FREE;
    free((void*)logbuff->base);
    logbuff->buff_destroyed_flag = LOGGER_SET;
    return LOGQUEUE_SUCCESS;
}

/* @brief adds an item to the logger queue
 *
 * add an item in memory to the logger queue
 * increment the head, and update all the flags
 *
 *
 * @param[in] LQ_t* the LQ_t object to operate on
 * @param[in] log_t* the object to add into the buffer
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_buffer_add_item(LQ_t* logbuff, log_t* data)
{
    if ((void*)logbuff == NULL)
        return LOGQUEUE_BAD_POINTER;
    /* if we have already destroyed the buffer */
    if ((LQ_f)logbuff->buff_destroyed_flag == LOGGER_SET)
        return LOGQUEUE_NO_BUFFER_IN_MEMORY;
    /* the buffer is already full */
    if ((LQ_f)logbuff->buff_full_flag == LOGGER_SET)
    {
        logbuff->buff_ovf_flag = LOGGER_SET;
        /* data is trashed */
        return LOGQUEUE_FULL;
    }
    END_CRITICAL();
    log_t* temp= (log_t*)malloc(sizeof(log_t));/*declare new log_t on heap*/
    temp->LogID = data->LogID;/*fill new heap data with stuff from data*/
    temp->ModuleID=data->ModuleID;
    temp->LogLength=data->LogLength;
    temp->Timestamp= data->Timestamp;
    temp->Checksum = data->Checksum;
    if(temp->LogLength>0)
    {
    /*if there's a payload, allocate space for it, copy it into heap memory, 
     * and then put the new heap allocation of it into the new heap log structure.
     * put the pointer to this heap-allocated structure into the circular buffer*/
        uint8_t* Payloadtemp= (uint8_t*)malloc(((int)temp->LogLength)*sizeof(uint8_t));
        my_memmove(data->PayloadData, Payloadtemp, data->LogLength);
        temp->PayloadData = Payloadtemp;
    }
    *(logbuff->head) = temp;
    logbuff->head = logbuff->head + 1; /* or just logbuff->head++; */
    logbuff->num_in++;
    logbuff->buff_empty_flag = LOGGER_UNSET;
    /* it's a circular buffer, so loop around if we go beyond the max */
    if (logbuff->head > (logbuff->base + (logbuff->buff_size - 1)))
    {
        logbuff->head = logbuff->base;
    }
    /* the buffer must be full since we added something and the following is true*/
    if (logbuff->head == logbuff->tail)
    {
        logbuff->buff_full_flag = LOGGER_SET;
        /* this should never happen */
        if (logbuff->num_in != logbuff->buff_size)
        {
            START_CRITICAL();
            return LOGQUEUE_CRITICAL_ERROR;
        }
    }
    START_CRITICAL();
    UART_start_buffered_transmission()
    return LOGQUEUE_SUCCESS;/*TODO change this to activate the UART interrupt*/
}

/* @brief removes an item from the logger queue
 *
 * removes an item from the logger queue by
 * incrementing the tail an decrementing the num_in
 *
 * @param[in] LQ_t* the LQ_t object to operate on
 * @param[out] log_t** put the data removed into this pointer
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_buffer_remove_item(LQ_t* logbuff, log_t** data)
{
    if ((void*)logbuff == NULL)
        return LOGQUEUE_BAD_POINTER;
    /* if we have already destroyed the buffer */
    if ((LQ_f)logbuff->buff_destroyed_flag == LOGGER_SET)
        return LOGQUEUE_NO_BUFFER_IN_MEMORY;
    if ((LQ_f)logbuff->buff_empty_flag == LOGGER_SET)
        return LOGQUEUE_EMPTY;
    END_CRITICAL();
    *data = *logbuff->tail;
    logbuff->tail = logbuff->tail + 1; /* or just logbuff->tail++; */
    logbuff->num_in--;
    logbuff->buff_full_flag = LOGGER_UNSET;
    /* it's a circular buffer, so loop around if we go beyond the max */
    if (logbuff->tail > (logbuff->base + (logbuff->buff_size - 1)))
    {
        logbuff->tail = logbuff->base;
    }
    /* the buffer must be empty since we removed something and the following is true */
    if (logbuff->head == logbuff->tail)
    {
        logbuff->buff_empty_flag = LOGGER_SET;
        /* this should never happen */

        if (logbuff->num_in != 0)
        {
            START_CRITICAL();
            return LOGQUEUE_CRITICAL_ERROR;
        }

    }
    START_CRITICAL();
    return LOGQUEUE_SUCCESS;
}

/* @brief checks to see if the buffer is full
 *
 * simply checks the full flag of the buffer to see
 * if the head ever passed the tail
 *
 * @param[in] LQ_t* the LQ_t object to operate on
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_is_full(LQ_t* logbuff)
{
    if ((void*)logbuff == NULL)
        return LOGQUEUE_BAD_POINTER;
    if ((LQ_f)logbuff->buff_full_flag == LOGGER_SET)
        return LOGQUEUE_FULL;
    return LOGQUEUE_SUCCESS;
}

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
LQ_e LQ_is_empty(LQ_t* logbuff)
{
    if ((void*)logbuff == NULL)
        return LOGQUEUE_BAD_POINTER;
    if ((LQ_f)logbuff->buff_empty_flag == LOGGER_SET)
        return LOGQUEUE_EMPTY;
    return LOGQUEUE_SUCCESS;
}

/* @brief returns the value in the buffer at a position back from the head
 *
 * peeking at the 0th value will just return the last value
 * that was put into the buffer.
 *
 * @param[in] LQ_t* the LQ_t object to operate on
 * @param[in] size_t the index away from the head
 * @param[out] log_t** put the data peeked at into this pointer
 * @return LQ_e This function returns the LQ_e typedef to indicate errors
 */
LQ_e LQ_peek(LQ_t* logbuff, size_t position, log_t** data)
{
    log_t** peekdata = (log_t**)logbuff->head;
    if ((void*)logbuff == NULL)
        return LOGQUEUE_BAD_POINTER;
    /* since head points at an empty value we increment by one */
    position++;
    if (position > logbuff->num_in)
        return LOGQUEUE_POSITION_TOO_LARGE;
    /* it's a circular buffer, so loop around if we go beyond the max */
    if (peekdata < logbuff->base)
    {
        peekdata = 1*(size_t)(logbuff->buff_size-1) + peekdata;
    }
    *data = *(log_t**)peekdata;
    return LOGQUEUE_SUCCESS;
}
