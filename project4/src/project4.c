/*
 * @file project4.c
 * @brief implements project4.h
 *
 *  Project 4 functionality
 *
 *  @author Seth Miers and Jake Cazden
 *  @date April 26, 2018
 */

#define _POSIX_C_SOURCE 199309L
#include "project4.h"
#include "logger.h"
#include "logger_queue.h"
#include "circbuf.h"
#include "memory.h"
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#ifdef KL25Z
#include "uart.h"
#include "MKL25Z4.h"
#include "port.h"
	#include "arch_arm32.h"
#endif

extern volatile uint32_t DMA_end_value;
extern volatile uint8_t dma_error_flag;
extern CB_t* recieve_buffer;
extern LQ_t* log_buffer;
extern volatile uint32_t nooperation;

void project4()
{
    uint8_t * my_string;
#ifdef KL25Z
    UART_configure();/*TODO put in better UART setup control*/
#endif
    logger_init();
#if defined(HOST) || defined(BBB)
    if(CB_init(recieve_buffer, CIRCBUF_HOST_LENGTH)!=SUCCESS) return;
#endif
    if(recieve_buffer==NULL)
    {
#ifdef LOGGING
        my_string = (uint8_t*) "recieve buffer failure";
        log_item((log_t){ERROR,FUNC_PROJECT4,22,0,my_string,0});
#endif
        return;
    }
    if(log_buffer==NULL)return;
#ifdef KL25Z
#ifdef KL25Z
    GPIO_Configure();
#endif
#ifdef LOGGING
    log_item((log_t){GPIO_INITIALIZED,FUNC_PROJECT4,0,0,NULL,0});
#endif
#ifdef LOGGING
    my_string = (uint8_t*)"GPIO is in use!!";
    log_item((log_t){WARNING,FUNC_PROJECT4,20,0,my_string,0});
#endif
#ifdef LOGGING
    log_item((log_t){SYSTEM_INITIALIZED,FUNC_PROJECT4,0,0,NULL,0});
#endif
#ifdef LOGGING
    uint16_t UIDMH = SIM_UIDMH;
    uint32_t UIDML = SIM_UIDML;
    uint32_t UIDL = SIM_UIDL;
    uint8_t UID[10];
    my_memcpy((uint8_t*)&UIDL,&UID[0],4);
    my_memcpy((uint8_t*)&UIDML,&UID[4],4);
    my_memcpy((uint8_t*)&UIDMH,&UID[8],2);
    log_item((log_t){SYSTEM_ID,FUNC_PROJECT4,10,0,UID,0});
    uint32_t vers = SIM_SDID;
    log_item((log_t){SYSTEM_VERSION,FUNC_PROJECT4,4,0,(uint8_t*)&vers,0});
#endif
#endif
    /*^^^ see  https://gcc.gnu.org/onlinedocs/gcc-4.3.2/gcc/Compound-Literals.html*/
    uint8_t data=0;
    CB_e retval=SUCCESS;
    uint8_t char_holder;
#if defined(HOST) || defined(BBB)
    char_holder=data;
    my_string = (unsigned char*) "Type a string to be processed, return to submit\n";
    log_item((log_t){INFO,FUNC_PROJECT4,48,0,my_string,0});
#endif
#ifdef LOGGING
        log_item((log_t){DATA_ANALYSIS_STARTED,FUNC_PROJECT4,0,0,NULL,0});
#endif
project4_profiler();
    while(1)
    {
    data=0;
    char_holder=0;
    nooperation+=char_holder;
    while( data!=ASCII_OFFSET_EOF && data!=EOF && data!=0xff && data!='~')
    {
#if defined(HOST) || defined(BBB)
    do
    {
        if(data==ASCII_OFFSET_EOF||data==EOF)break;
        if(char_holder==ASCII_OFFSET_EOF||char_holder==EOF||data!=char_holder)break;
            char_holder = (uint8_t)getchar();
            CB_buffer_add_item(recieve_buffer,char_holder);
    }while(char_holder!='\n'&&char_holder!='\r'&&char_holder!=ASCII_OFFSET_EOF&&char_holder!=EOF&&char_holder!=0xff);
#endif
        retval=CB_buffer_remove_item(recieve_buffer, &data);
        if(retval==SUCCESS)
        {
#ifdef LOGGING
            log_item((log_t){DATA_RECIEVED,FUNC_UART,1,0,&data,0});
#endif
            if(data>=ASCII_OFFSET_0 && data<=ASCII_OFFSET_9)
            {
                statistics.numeric++;
            }
            else if(  (data>=ASCII_OFFSET_A && data<=ASCII_OFFSET_Z)
                    |(data>=ASCII_OFFSET_LA && data<=ASCII_OFFSET_LZ))
            {
                statistics.alphabetic++;
            }
            else if( (data==(uint8_t)'\'')|(data==(uint8_t)'[' )|(data==(uint8_t)']')
                    |(data==(uint8_t)'{' )|(data==(uint8_t)'}' )|(data==(uint8_t)'(')
                    |(data==(uint8_t)')' )|(data==(uint8_t)'<' )|(data==(uint8_t)'>')
                    |(data==(uint8_t)':' )|(data==(uint8_t)',' )|(data==(uint8_t)'.')
                    |(data==(uint8_t)'!' )|(data==(uint8_t)'-' )|(data==(uint8_t)'?')
                    |(data==(uint8_t)'"' )|(data==(uint8_t)';' )|(data==(uint8_t)'/'))
            {
                statistics.punctuation++;
            }
            else
            {
                statistics.miscellaneous++;
            }
        }
    }
#ifdef LOGGING
    log_item((log_t){DATA_ALPHA_COUNT,FUNC_PROJECT4,4,0,(uint8_t*) &statistics.alphabetic,0});
    log_item((log_t){DATA_NUMERIC_COUNT,FUNC_PROJECT4,4,0,(uint8_t*) &statistics.numeric,0});
    log_item((log_t){DATA_PUNCTUATION_COUNT,FUNC_PROJECT4,4,0,(uint8_t*) &statistics.punctuation,0});
    log_item((log_t){DATA_MISC_COUNT,FUNC_PROJECT4,4,0,(uint8_t*) &statistics.miscellaneous,0});
    log_item((log_t){DATA_ANALYSIS_COMPLETED,FUNC_PROJECT4,0,0,NULL,0});
#endif
    project4_dump_statistics();
    }
}

void project4_dump_statistics()
{
    #ifdef LOGGING
    uint8_t* loggererror = (uint8_t*)"logger_error";
    #endif
    uint8_t * my_string;
    volatile uint8_t bufferstring[32] = {};
    uint8_t * stringnewline = (unsigned char*)"\r\n";
    my_string = (unsigned char*) "\r\nStatistics:";
    log_ret retval = log_item((log_t){INFO,FUNC_PROJECT4,14,0,my_string,0});
    #ifndef LOGGING
    nooperation+=(uint32_t)retval;
    #endif
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif
    retval = log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif

    my_string = (unsigned char*) "\tAlphabetic Characters: ";
    retval = log_item((log_t){INFO,FUNC_PROJECT4,24,0,my_string,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif
    volatile uint32_t length = my_itoa(statistics.alphabetic,(uint8_t*)bufferstring,10);
    retval = log_item((log_t){INFO,FUNC_PROJECT4,length,0,(uint8_t*)bufferstring,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif
    retval = log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif

    my_string = (unsigned char*) "\tNumeric Characters: ";
    retval = log_item((log_t){INFO,FUNC_PROJECT4,21,0,my_string,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif
    length = my_itoa(statistics.numeric,(uint8_t*)bufferstring,10);
    retval = log_item((log_t){INFO,FUNC_PROJECT4,length,0,(uint8_t*)bufferstring,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif
    retval = log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif

    my_string = (unsigned char*) "\tPunctuation Characters: ";
    retval = log_item((log_t){INFO,FUNC_PROJECT4,25,0,my_string,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif
    length = my_itoa(statistics.punctuation,(uint8_t*)bufferstring,10);
    retval = log_item((log_t){INFO,FUNC_PROJECT4,length,0,(uint8_t*)bufferstring,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif
    retval = log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif

    my_string = (unsigned char*) "\tMiscellaneous Characters: ";
    retval = log_item((log_t){INFO,FUNC_PROJECT4,27,0,my_string,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif
    length = my_itoa(statistics.miscellaneous,(uint8_t*)bufferstring,10);
    retval = log_item((log_t){INFO,FUNC_PROJECT4,length,0,(uint8_t*)bufferstring,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif
    retval = log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});
#ifdef LOGGING
    if(retval==LOGGER_FAILURE)log_item((log_t) {ERROR,FUNC_LOGGER,12,0,loggererror,0});
#endif

    log_flush();
}
#ifdef KL25Z
void project4_profiler()
{
#ifdef LOGGING
    log_item((log_t){PROFILING_STARTED,FUNC_PROJECT4,0,0,NULL,0});
#endif
	/* TODO buildtime settings must be adjusted so that the heap can support these regions (at least 1kB) */
	/* TODO refactor this code to be smaller */
	/* TODO run with highest optimizations and lowest optimizations to see difference */

	/* areas to copy and move to */
	uint8_t *area_one = (uint8_t *) malloc(sizeof(uint8_t)*512);
	uint8_t *area_two = area_one+20;
    uint8_t *retval = NULL;

	/* timer values */
	volatile uint32_t start_value = 0;
	volatile uint32_t end_value = 0;

	/* strings used for printing */
    uint8_t *my_string;
    uint8_t my_string_buffer[128];
    uint8_t num_string[16];
	uint8_t printsize = 0x00;

	/*******************/
	/* MEMSET PROFILER */
	/*******************/

	/* standard library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memset library with 10 bytes took ";
	my_memcpy(my_string,my_string_buffer, 57);
	start_value = SysTick_Base_Ptr->CVR;
	memset(area_one, '1', 10);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	my_memcpy(num_string,my_string_buffer+57, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	my_memcpy(my_string,my_string_buffer+printsize+57, 22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,57+printsize+22,0,my_string_buffer,0});
#endif

	/* my library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memset function with 10 bytes took ";
	my_memcpy(my_string,my_string_buffer, 56);
	start_value = SysTick_Base_Ptr->CVR;
	my_memset(area_one, 10, '1');
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	my_memcpy(num_string,my_string_buffer+56, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	my_memcpy(my_string,my_string_buffer+printsize+56, 22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,56+printsize+22,0,my_string_buffer,0});
#endif
	/* dma library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memset function with 10 bytes took ";
	my_memcpy(my_string,my_string_buffer, 53);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memset_dma(area_one, 10, '1', 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = '-';
        num_string[1] = '1';
        printsize = 2;
    }
	my_memcpy(num_string,my_string_buffer+53, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	my_memcpy(my_string,my_string_buffer+printsize+53, 22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,53+printsize+22,0,my_string_buffer,0});
#endif

	/********************/
	/* MEMMOVE PROFILER */
	/********************/
	/* standard library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memmove library with 10 bytes took ";
	my_memcpy(my_string,my_string_buffer, 58);
	start_value = SysTick_Base_Ptr->CVR;
	memmove(area_one, area_two, 10);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	my_memcpy(num_string,my_string_buffer+58, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	my_memcpy(my_string,my_string_buffer+printsize+58, 22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,58+printsize+22,0,my_string_buffer,0});
#endif
	/* my library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memmove function with 10 bytes took ";
	my_memcpy(my_string,my_string_buffer, 57);
	start_value = SysTick_Base_Ptr->CVR;
	my_memmove(area_one, area_two, 10);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	my_memcpy(num_string,my_string_buffer+57, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	my_memcpy(my_string,my_string_buffer+printsize+57, 22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,57+printsize+22,0,my_string_buffer,0});
#endif
	/* dma library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memmove function with 10 bytes took ";
	my_memcpy(my_string,my_string_buffer, 54);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memmove_dma(area_one, area_two, 10, 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = '-';
        num_string[1] = '1';
        printsize = 2;
    }
	my_memcpy(num_string,my_string_buffer+54, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	my_memcpy(my_string,my_string_buffer+printsize+54, 22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,54+printsize+22,0,my_string_buffer,0});
#endif
#ifdef LOGGING
    log_item((log_t){PROFILING_COMPLETED,FUNC_PROJECT4,0,0,NULL,0});
#endif
    nooperation+=(uint32_t)retval;
}
#endif
#if defined(BBB) || defined(HOST)
void project4_profiler()
{
	/* TODO buildtime settings must be adjusted so that the heap can support these regions (at least 1kB) */
	/* TODO refactor this code to be smaller */
	/* TODO run with highest optimizations and lowest optimizations to see difference */

	/* areas to copy and move to */
	uint8_t *area_one = (uint8_t *) malloc(sizeof(uint8_t)*512);
	uint8_t *area_two = area_one+20;
    uint8_t* my_string;
    uint8_t my_string_buffer[128];
    uint8_t my_num_buffer[32];

	/* timer values */
	struct timespec start_value;
	struct timespec end_value;

	/*******************/
	/* MEMSET PROFILER */
	/*******************/
#ifdef LOGGING
    log_item((log_t){PROFILING_STARTED,FUNC_PROJECT4,0,0,NULL,0});
#endif

	/* standard library */
	/* 10 bytes */
    my_string = (uint8_t*)"Profiling the standard memset library with 10 bytes took ";
    my_memcpy(my_string,my_string_buffer,57);
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memset(area_one, '1', 10);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	uint16_t len = my_itoa((end_value.tv_nsec-start_value.tv_nsec)*3/10,my_num_buffer,10);
    my_memcpy(my_num_buffer,my_string_buffer+57,len);
    my_string = (uint8_t*)" clock cycles to run\r\n";
    my_memcpy(my_string,my_string_buffer+57+len,22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,57+len+22,0,my_string_buffer,0});
#endif
	/* 100 bytes */
    my_string = (uint8_t*)"Profiling the standard memset library with 100 bytes took ";
    my_memcpy(my_string,my_string_buffer,58);
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memset(area_one, '2', 100);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	len = my_itoa((end_value.tv_nsec-start_value.tv_nsec)*3/10,my_num_buffer,10);
    my_memcpy(my_num_buffer,my_string_buffer+58,len);
    my_string = (uint8_t*)" clock cycles to run\r\n";
    my_memcpy(my_string,my_string_buffer+58+len,22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,58+len+22,0,my_string_buffer,0});
#endif
	/* my library */
	/* 10 bytes */
    my_string = (uint8_t*)"Profiling the custom memset function with 10 bytes took ";
    my_memcpy(my_string,my_string_buffer,56);
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memset(area_one, 10, '1');
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	len = my_itoa((end_value.tv_nsec-start_value.tv_nsec)*3/10,my_num_buffer,10);
    my_memcpy(my_num_buffer,my_string_buffer+56,len);
    my_string = (uint8_t*)" clock cycles to run\r\n";
    my_memcpy(my_string,my_string_buffer+56+len,22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,56+len+22,0,my_string_buffer,0});
#endif
	/* 100 bytes */
    my_string = (uint8_t*)"Profiling the custom memset function with 100 bytes took ";
    my_memcpy(my_string,my_string_buffer,57);
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memset(area_one, 100, '2');
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	len = my_itoa((end_value.tv_nsec-start_value.tv_nsec)*3/10,my_num_buffer,10);
    my_memcpy(my_num_buffer,my_string_buffer+57,len);
    my_string = (uint8_t*)" clock cycles to run\r\n";
    my_memcpy(my_string,my_string_buffer+57+len,22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,57+len+22,0,my_string_buffer,0});
#endif

	/********************/
	/* MEMMOVE PROFILER */
	/********************/
	/* standard library */
	/* 10 bytes */
    my_string = (uint8_t*)"Profiling the standard memmove library with 10 bytes took ";
    my_memcpy(my_string,my_string_buffer,58);
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memmove(area_one, area_two, 10);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	len = my_itoa((end_value.tv_nsec-start_value.tv_nsec)*3/10,my_num_buffer,10);
    my_memcpy(my_num_buffer,my_string_buffer+58,len);
    my_string = (uint8_t*)" clock cycles to run\r\n";
    my_memcpy(my_string,my_string_buffer+58+len,22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,58+len+22,0,my_string_buffer,0});
#endif
	/* 100 bytes */
    my_string = (uint8_t*)"Profiling the standard memmove library with 100 bytes took ";
    my_memcpy(my_string,my_string_buffer,59);
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memmove(area_one, area_two, 100);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	len = my_itoa((end_value.tv_nsec-start_value.tv_nsec)*3/10,my_num_buffer,10);
    my_memcpy(my_num_buffer,my_string_buffer+59,len);
    my_string = (uint8_t*)" clock cycles to run\r\n";
    my_memcpy(my_string,my_string_buffer+59+len,22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,59+len+22,0,my_string_buffer,0});
#endif
	/* my library */
	/* 10 bytes */
    my_string = (uint8_t*)"Profiling the custom memmove function with 10 bytes took ";
    my_memcpy(my_string,my_string_buffer,57);
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memmove(area_one, area_two, 10);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	len = my_itoa((end_value.tv_nsec-start_value.tv_nsec)*3/10,my_num_buffer,10);
    my_memcpy(my_num_buffer,my_string_buffer+57,len);
    my_string = (uint8_t*)" clock cycles to run\r\n";
    my_memcpy(my_string,my_string_buffer+57+len,22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,57+len+22,0,my_string_buffer,0});
#endif
	/* 100 bytes */
    my_string = (uint8_t*)"Profiling the custom memmove function with 100 bytes took ";
    my_memcpy(my_string,my_string_buffer,58);
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memmove(area_one, area_two, 100);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	len = my_itoa((end_value.tv_nsec-start_value.tv_nsec)*3/10,my_num_buffer,10);
    my_memcpy(my_num_buffer,my_string_buffer+58,len);
    my_string = (uint8_t*)" clock cycles to run\r\n";
    my_memcpy(my_string,my_string_buffer+58+len,22);
#ifdef LOGGING
    log_item((log_t){PROFILING_RESULT,FUNC_PROJECT4,58+len+22,0,my_string_buffer,0});
#endif
#ifdef LOGGING
    log_item((log_t){PROFILING_COMPLETED,FUNC_PROJECT4,0,0,NULL,0});
#endif
}
#endif
