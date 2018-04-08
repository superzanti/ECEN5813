/**
 * @file memory.c
 * @brief implements memory.h
 *
 * implements software defined functions for copying, allocating, and freeing
 * memory, without relying on hardware specifically.
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2018
 *
 */
#include "memory.h"

/* Only need the following if we cant use
 * built in malloc and free functions
 * extern void __HeapBase, __HeapLimit;
 */

/* Needed to return null pointers
 * This is defined in stddef so it's commented out
 *
 *  #ifndef NULL
 *  #define NULL ((void*)0)
 *  #endif
 */

#include <stdlib.h>
#include <stddef.h>
#ifdef KL25Z
#include "MKL25Z4.h"
#include "arch_arm32.h"
extern uint8_t dma_first_setup;
#endif
extern volatile uint8_t dma_error_flag;

extern uint8_t dma0_done;
extern volatile uint32_t DMA_end_value;
extern volatile uint32_t nooperation;
uint8_t * memset_dma(uint8_t * src, size_t length, uint8_t value, size_t transfer)
{
	/* TODO implement function */
	#ifdef KL25Z
    dma0_done=0;
    uint32_t valholder= (value)+(value<<8)+(value<<16)+(value<<24);
    DMA_e retval = setup_memtransfer_dma((uint8_t *)&valholder, 1, src, transfer, length);
    while(dma0_done==0 && retval==DMA_SUCCESS)
    {
        nooperation++;
    }
    if(retval==DMA_ERROR)
    {
        return NULL;
    }
	return src;
	#else
	return my_memset(src, length, value);
	#endif
}

uint8_t * memmove_dma(uint8_t * src, uint8_t * dst, size_t length, size_t transfer)
{
	#ifdef KL25Z
    if(dst>src && dst<src+length)
    {/*the case where overlap will occur, but isn't exactly on top of itself*/
        dma0_done=0;
        uint32_t templength1 = ((uint32_t)src+length)-(uint32_t)dst;
        uint32_t templength2 = (uint32_t)dst-(uint32_t)src;
        DMA_e retval = setup_memtransfer_dma(dst, templength1, src+length, transfer, templength1);
        while(dma0_done==0 && retval==DMA_SUCCESS)
        {
            nooperation++;
        }
        if(retval==DMA_ERROR)
        {
            return NULL;
        }
        dma0_done=0;
        retval = setup_memtransfer_dma(src, templength2, dst, transfer, templength2);
        while(dma0_done==0 && retval==DMA_SUCCESS)
        {
            nooperation++;
        }
        if(retval==DMA_ERROR)
        {
            return NULL;
        }
    }
    else
    {
        dma0_done=0;
        DMA_e retval = setup_memtransfer_dma(src, length, dst, transfer, length);
        while(dma0_done==0 && retval==DMA_SUCCESS)
        {
            nooperation++;
        }
        if(retval==DMA_ERROR)
        {
            return NULL;
        }
    }
	return dst;
	#else
	return my_memmove(src, dst, length);
	#endif
}

uint8_t * my_memmove(uint8_t * src, uint8_t * dst, size_t length)
{
    /* check for null pointers */
    if(!dst)
    {
        return NULL;
    }
    if(!src)
    {
        return NULL;
    }
    /* create a new array to copy into to prevent corruption */
    uint8_t my_array[length];
    /* copy our current array into the new array */
    my_memcpy(src, my_array, length);
    /* copy the new array into the destination array*/
    return my_memcpy(my_array, dst, length);
}

uint8_t * my_memcpy(uint8_t * src, uint8_t * dst, size_t length)
{
    /* check for null pointers */
    if(!dst)
    {
        return NULL;
    }
    if(!src)
    {
        return NULL;
    }
    /* create a variable to iterate through (for loop) */
    size_t i = 0;
    /* save the original destination since we'll be changing its value */
    uint8_t* orig_dst = dst;
    /* loop for how many bytes there are */
    for(i=0;i<length;i++)
    {
        /* copy the source to the destination */
        *dst++ = *src++;
    }
    return orig_dst;
}

uint8_t * my_memset(uint8_t * src, size_t length, uint8_t value)
{
    /* check for null pointers */
    if(!src)
    {
        return NULL;
    }
    /* create a variable to iterate through (for loop) */
    size_t i = 0;
    /* loop for how many bytes there are */
    for(i=0;i<length;i++)
    {
        /* copy the value into the source */
        *src++ = value;
    }
    return src;
}

uint8_t * my_memzero(uint8_t * src, size_t length)
{
    /* just call my_memset with a value of 0 */
    return my_memset(src, length, 0); 
}

uint8_t * my_reverse(uint8_t * src, size_t length)
{
    /* check for null pointers */
    if(!src)
    {
        return NULL;
    }
    /* create a temporary array */
    uint8_t my_array[length];
    /* copy the source into the temporary array */
    my_memcpy(src, my_array, length);
    /* create an address that points to the end of the array */
    uint8_t * my_array_ptr = &my_array[length-1];
    /* loop for how many bytes there are */
    size_t i = 0;
    for(i=0;i<length;i++)
    {
        /* copy the destination of the pointer
         * of the end of the array into the srouce */
        *src++ = *my_array_ptr--;
    }
    return src;
}

void * reserve_words(size_t length)
{
    void* src;
    /* malloc will return null if it fails */
    src = (void*)malloc(length*sizeof(void*));
    return src;
}

uint8_t free_words(void * src)
{
    free(src);
    return 0;
}
#ifdef KL25Z
DMA_e setup_memtransfer_dma(uint8_t* src, uint8_t src_len, uint8_t* dst,
                            size_t transfersize, size_t length)/*, uint8_t dma_index)*/
{
    /*if(dma_index>4) return BAD_INDEX;*/
    if(src_len==0 || length==0) return DMA_NO_LENGTH;
    if(src==NULL || dst==NULL) return DMA_BAD_POINTER;
    if(dma_first_setup==0)
    {
        /*turn on clock gates to the mux and dma modules*/
        SIM_SCGC6 |= SIM_SCGC6_DMAMUX(DMAMUX_CLOCKGATE_ENABLE);
        SIM_SCGC7 |= SIM_SCGC7_DMA(DMA_CLOCKGATE_ENABLE);
        dma_first_setup=1;
    }
    /*if((DMA_DSR_BCR0&DMA_DSR_BCR_DONE_MASK)>>DMA_DSR_BCR_DONE_SHIFT!=1)
    {
        if a transfer is active, error out TODO this could optionally
          search for an open channel instead
        return DMA_BUSY;
    }*/
    /*enable dma channel 0, set it to non-periodic, and attach it to an always
     * active request source*/
    /*clear errors and disable active transfer*/
    DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE(DMA_DSR_BCR_DONE_WRITETOCLEAR);
    DMAMUX0_CHCFG0 = DMAMUX_CHCFG_ENBL(DMAMUX_CHCFG_ENABLE)
                        |DMAMUX_CHCFG_TRIG(DMAMUX_CHCFG_SINGLETRIGGER)
                        |DMAMUX_CHCFG_SOURCE(DMAMUX_CHCFG_SOURCE_ALWAYSON_60);
    uint32_t checkaddr = ((uint32_t)src&(uint32_t)0xfff00000)>>20;
    if(checkaddr==0x000 || checkaddr==0x1ff || checkaddr==0x200 || checkaddr==0x400)
    {
        /*if the source is allowed, put it  into the source register*/
        DMA_SAR0 = (uint32_t)src;
    }
    else return DMA_BAD_POINTER;
    checkaddr = ((uint32_t)dst&(uint32_t)0xfff00000)>>20;
    if(checkaddr==0x000 || checkaddr==0x1ff || checkaddr==0x200 || checkaddr==0x400)
    {
        /*if the destination is allowed, put it into the destination register*/
        DMA_DAR0 = (uint32_t)dst;
    }
    else return DMA_BAD_POINTER;
    /*if the transfer is too large, turn it off*/
    if(length<DMA_DSR_BCR_BCRMAXVALUE)
    {
        /*put the number of bytes into the byte count register*/
        /*TODO might need to clear the BCR register as well.*/
        DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(length);
    }
    else return DMA_BCR_LENGTH_OVERFLOW;
    /*enable interrupt on complete or error, keep peripheral requests off,
     * allow for continuous (non-cycle-steal) operation, dont auto-align
     * dont allow asynchronous requests during low power, enable the transfer
     * disable writing into and from a circular buffer, turn off the ability of
     * the dma to disable it's source request (peripheral operation only)
     * and turn off channel linking. set the destination to increment.*/
    uint32_t DCRregwrite = DMA_DCR_EINT(DMA_DCR_INTERRUPT_ON_COMPLETE)
                        |DMA_DCR_ERQ(DMA_DCR_NO_PERIPHERAL_REQUEST)
                        |DMA_DCR_CS(DMA_DCR_CONTINUOUS_OPERATION)
                        |DMA_DCR_EADREQ(DMA_DCR_NO_ASYNCH_REQUESTS)
                        |DMA_DCR_START(DMA_DCR_START_ENABLE)
                        |DMA_DCR_SMOD(DMA_DCR_NO_SOURCE_MODULO)
                        |DMA_DCR_DMOD(DMA_DCR_NO_DEST_MODULO)
                        |DMA_DCR_D_REQ(DMA_DCR_DISABLE_REQUEST_OFF)
                        |DMA_DCR_LINKCC(DMA_DCR_CHANNEL_LINK_DISABLED)
                        |DMA_DCR_DINC(DMA_DCR_INCREMENT_DEST);
    if(src_len==1)/*set the source to increment iff there is more than  1 byte of source*/
    {
        DCRregwrite|=DMA_DCR_SINC(DMA_DCR_NO_SOURCE_INCREMENT);
    }
    else
    {
        DCRregwrite|=DMA_DCR_SINC(DMA_DCR_INCREMENT_SOURCE);
    }
    if(transfersize==1 || length<=16)/*set transfer size if passed appropriately*/
    {
        DCRregwrite|=DMA_DCR_SSIZE(DMA_DCR_TRANSFERSIZE_8BIT)
                    |DMA_DCR_DSIZE(DMA_DCR_TRANSFERSIZE_8BIT)
                    |DMA_DCR_AA(DMA_DCR_NO_AUTOALIGN);
    }
    else if(transfersize==2)
    {
        DCRregwrite|=DMA_DCR_SSIZE(DMA_DCR_TRANSFERSIZE_16BIT)
                    |DMA_DCR_DSIZE(DMA_DCR_TRANSFERSIZE_16BIT)
                    |DMA_DCR_AA(DMA_DCR_AUTOALIGN);
    }
    else if(transfersize==4)
    {
        DCRregwrite|=DMA_DCR_SSIZE(DMA_DCR_TRANSFERSIZE_32BIT)
                    |DMA_DCR_DSIZE(DMA_DCR_TRANSFERSIZE_32BIT)
                    |DMA_DCR_AA(DMA_DCR_AUTOALIGN);
    }
    else return DMA_BAD_SIZE;

    NVIC_ClearPendingIRQ(DMA0_IRQn);/*enable interrupts on DMA0 ocmplete and error*/
    NVIC_EnableIRQ(DMA0_IRQn);
    __enable_irq();

    DMA_DCR0=DCRregwrite;/*write DMA control register to start transfer*/
    return DMA_SUCCESS;
}
#endif
#ifdef KL25Z
void DMA0_IRQHandler()
{
	DMA_end_value = SysTick_Base_Ptr->CVR;
    NVIC_ClearPendingIRQ(DMA0_IRQn);
    NVIC_DisableIRQ(DMA0_IRQn);
    DMAMUX0_CHCFG0 = 0;
    DMA_DCR0=0;
    DMA_SAR0=0;
    DMA_DAR0=0;
    if(DMA_DSR_BCR(0)&(DMA_DSR_BCR_CE_MASK))
    {
        dma_error_flag=1;
    }
    DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE(DMA_DSR_BCR_DONE_WRITETOCLEAR);
    dma0_done=1;
}
#endif
