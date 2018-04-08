/**
 * @file memory.h
 * @brief contains functions for allocating, freeing, and copying memory
 *
 * this contains numerous functions for the allocation, movement, and freeing
 * of memory, utilizing software, not hardware specific functionality.
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2018
 *
 */
#ifndef __MEMORY_H__
#define __MEMORY_H__

/* Type definitions needed for function prototypes */
#include <stdint.h>
#include <stddef.h>

#define DMAMUX_CLOCKGATE_ENABLE         (1)
#define DMA_CLOCKGATE_ENABLE            (1)
#define DMAMUX_CHCFG_SINGLETRIGGER      (0)
#define DMAMUX_CHCFG_DISABLE            (0)
#define DMAMUX_CHCFG_ENABLE             (1)
#define DMAMUX_CHCFG_SOURCE_ALWAYSON_60 (0x3C)
#define DMA_DSR_BCR_DONE_WRITETOCLEAR   (1)
#define DMA_DSR_BCR_BCRMAXVALUE         (0x0FFFFF)
#define DMA_DCR_INTERRUPT_ON_COMPLETE   (1)
#define DMA_DCR_NO_PERIPHERAL_REQUEST   (0)
#define DMA_DCR_CONTINUOUS_OPERATION    (0)
#define DMA_DCR_NO_AUTOALIGN            (0)
#define DMA_DCR_NO_ASYNCH_REQUESTS      (0)
#define DMA_DCR_NO_SOURCE_INCREMENT     (0)
#define DMA_DCR_INCREMENT_SOURCE        (1)
#define DMA_DCR_TRANSFERSIZE_32BIT      (0)
#define DMA_DCR_TRANSFERSIZE_8BIT       (1)
#define DMA_DCR_TRANSFERSIZE_16BIT      (2)
#define DMA_DCR_NO_DEST_INCREMENT       (0)
#define DMA_DCR_INCREMENT_DEST          (1)
#define DMA_DCR_START_DISABLE           (0)
#define DMA_DCR_START_ENABLE            (1)
#define DMA_DCR_NO_SOURCE_MODULO        (0)
#define DMA_DCR_NO_DEST_MODULO          (0)
#define DMA_DCR_DISABLE_REQUEST_OFF     (0)
#define DMA_DCR_CHANNEL_LINK_DISABLED   (0)

typedef enum {
    DMA_SUCCESS,
    DMA_BAD_INDEX,
    DMA_BAD_POINTER,
    DMA_NO_LENGTH,
    DMA_BCR_LENGTH_OVERFLOW,
    DMA_BAD_SIZE,
    DMA_BUSY
} DMA_e;

/**
 * @brief function to set an array of bytes all to the same value using DMA.
 *
 * This function takes in a poitner to an array of bytes
 * then for a given number of bytes, writes a value to each
 * of those bytes. This funciton uses DMA requests
 * to do the writes. This is designed to work on 1, 2, and 4
 * byte transfer sizes. This will trigger an interrupt when completed.
 *
 * This is currently only supported for the KL25Z FRDM Board
 *
 * @param src a pointer to the array of bytes that we will write over
 * @param length the number of bytes to iterate through
 * @param value the value to write to every byte in the array
 * @return uint8_t a pointer to the source (simply returns src)
 */
uint8_t * memset_dma(uint8_t * src, size_t length, uint8_t value,size_t transfer);

/**
 * @brief function to copy one byte array to another (overlap) using DMA.
 *
 * This function takes in 2 pointers to byte arrays.
 * A source and a destination pointer are provided
 * to the function as well as the length of the source bytes
 * so the function knows how many to move over.
 * This function handles overlap so that the data moved
 * does not become corrupted. this function uses DMA requests
 * to do the memory moves. This is designed to work on 1, 2, and 4
 * byte transfer sizes. This will trigger an interrupt when completed.
 *
 * This is currently only supported for the KL25Z FRDM Board
 *
 * @param src a pointer to the array of bytes that we will move from
 * @param dst a pointer to the array of bytes that we will move to
 * @param length the number of bytes to move
 * @return uint8_t a pointer to the destination
 */
uint8_t * memmove_dma(uint8_t * src, uint8_t * dst, size_t length,size_t transfer);

/**
 * @brief function to copy one byte array to another (overlap)
 * 
 * This function takes in 2 pointers to byte arrays.
 * A source and a destination pointer are provided
 * to the function as well as the length of the source bytes
 * so the function knows how many to move over.
 * This function handles overlap so that the data moved
 * does not become corrupted.
 *
 * This function is implemented
 * using only pointer arithmetic.
 *
 * @param src a pointer to the array of bytes that we will move from
 * @param dst a pointer to the array of bytes that we will move to
 * @param length the number of bytes to move
 * @return uint8_t a pointer to the destination
 */
uint8_t * my_memmove(uint8_t * src, uint8_t * dst, size_t length);

/**
 * @brief function to copy one byte array to another (no overlap)
 * 
 * This function takes in 2 pointers to byte arrays.
 * A source and a destination pointer are provided
 * to the function as well as the length of the source bytes
 * so the function knows how many to copy over.
 * This function does not handle overlap, so data can become corrupted.
 *
 * This function is implemented
 * using only pointer arithmetic.
 *
 * @param src a pointer to the array of bytes that we will copy from
 * @param dst a pointer to the array of bytes that we will copy to
 * @param length the number of bytes to copy
 * @return uint8_t a pointer to the destination
 */
uint8_t * my_memcpy(uint8_t * src, uint8_t * dst, size_t length);

/**
 * @brief function to set an array of bytes all to the same value
 * 
 * This function takes in a poitner to an array of bytes
 * then for a given number of bytes, writes a value to each
 * of those bytes.
 *
 * This function is implemented
 * using only pointer arithmetic.
 *
 * @param src a pointer to the array of bytes that we will write over
 * @param length the number of bytes to iterate through
 * @param value the value to write to every byte in the array
 * @return uint8_t a pointer to the source (simply returns src)
 */
uint8_t * my_memset(uint8_t * src, size_t length, uint8_t value);

/**
 * @brief function to write 0 to an array of bytes
 * 
 * This function takes in a pointer to an array of bytes
 * then for a given number of bytes, writes zero to each
 * of those bytes.
 *
 * This function is implemented
 * using only pointer arithmetic.
 *
 * @param src a pointer to the array of bytes that we will write to 0
 * @param length the number of bytes to write
 * @return uint8_t a pointer to the source (simply returns src)
 */
uint8_t * my_memzero(uint8_t * src, size_t length);

/**
 * @brief function to reverse an array of bytes
 * 
 * This function takes in a pointer to an array of bytes
 * and the length of the array. It then reverses all of the
 * bytes so the first byte becomes the last and the 
 * last byte becomes the first.
 *
 * This function is implemented
 * using only pointer arithmetic.
 *
 * @param src a pointer to the array of bytes that we will reverse
 * @param length the number of bytes to reverse
 * @return uint8_t a pointer to the source (simply returns src)
 */
uint8_t * my_reverse(uint8_t * src, size_t length);

/**
 * @brief function to allowcate an array of size_t
 * 
 * create an area in dynamic memory that is
 * reserved for future used. Specifically, this
 * function creates an array of size_t of length
 * length.
 *
 * This function is implemented
 * using only pointer arithmetic.
 *
 * @param length the length of the array to create
 * @return void a pointer to the memory location in dynamic memory
 */
void * reserve_words(size_t length);

/**
 * @brief function to free the dynamic memory created
 * 
 * This function frees the dynamic memory created,
 * usually created by reserve_words. This function
 * makes sure that the memory can be used or
 * reserved in the future. The function then
 * returns if it was successful.
 *
 * This function is implemented
 * using only pointer arithmetic.
 *
 * @param src a pointer to the start of the memory we need to free
 * @return uint8_t an error code (0 or 1) iff the operation was successful
 */
uint8_t free_words(void * src);

/*
 * @brief function to setup dma for different transfers
 *
 * this function takes in the DMA to use, address(s), transfer size,
 * and asociated DMA information and configures the registers properly to utilize DMA
 *
 * @param
 * @return DMA_e, this function returns an error type related to improper dma configuration
 * */
DMA_e setup_memtransfer_dma(uint8_t* src, uint8_t src_len, uint8_t* dst,
                            size_t transfersize, size_t length);/*, uint8_t dma_index)*/

/*
 * @brief DMA IRQ handler, for use with the DMA complete trigger
 *
 * @param none, this function takes no inputs
 * @return void, this function has no return value
 * */
void DMA0_IRQHandler();
#endif /* __MEMORY_H__ */
