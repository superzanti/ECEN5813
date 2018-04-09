/*
 * @file project3.c
 * @brief implements project3.h
 *
 *	Project 3 functionality
 *
 *  @author Seth Miers and Jake Cazden
 *  @date March 15, 2018
 */

#define _POSIX_C_SOURCE 199309L
#include "project3.h"

#include "circbuf.h"
#include "conversion.h"
#include "memory.h"
#include <string.h>
#ifdef BBB
	#include "nordic.h"
	#include "spi.h"
    #include <time.h>
    #include <stdio.h>
#endif
#ifdef KL25Z
	#include "nordic.h"
	#include "spi.h"
	#include "port.h"
	#include "uart.h"
	#include "arch_arm32.h"
#endif

extern CB_t* recieve_buffer;
extern CB_t* transmit_buffer;

extern volatile uint32_t DMA_end_value;
extern volatile uint8_t dma_error_flag;
extern volatile uint32_t nooperation;

/* start of stack, end of stack */
extern int __StackTop, __StackLimit;

void project3()
{
#ifdef KL25Z
	GPIO_Configure();
	SPI_init();
    UART_configure();
#endif
#if defined(KL25Z) || defined (BBB)
#ifdef KL25Z
	stack_tracker_init();
#endif
	spi_setup_and_test();
	profiler();
#ifdef KL25Z
	stackusage();
#endif
#else
    nooperation++;
#endif
}

#ifdef KL25Z
void stack_tracker_init()
{
	uint8_t dummy_stack_var = 0xAA;
	uint8_t *start_of_unused_stack = &dummy_stack_var;
	uint8_t * stackend = (uint8_t *) (&__StackLimit);
	while(start_of_unused_stack >= stackend)
	{
		*start_of_unused_stack = 0xAA;
		start_of_unused_stack--;
	}
}

void stackusage()
{
	uint8_t * stackend = (uint8_t *) (&__StackLimit);
	uint8_t * stackbegin = (uint8_t *) (&__StackTop);
	uint8_t * stackused = stackend;
	uint8_t * my_string;
    uint8_t num_string[16];
	uint8_t printsize = 0x00;
	while(*stackused==0xAA)
	{
		stackused++;
	}
	printsize = my_itoa((int32_t)(stackbegin-stackused), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " bytes of the stack used out of a ";
	UART_send_n(my_string, 34);
	printsize = my_itoa((int32_t)(stackbegin-stackend), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " total bytes (at max not current)\r\n";
	UART_send_n(my_string, 35);
}

void profiler()
{
	/* TODO buildtime settings must be adjusted so that the heap can support these regions (at least 1kB) */
	/* TODO refactor this code to be smaller */
	/* TODO run with highest optimizations and lowest optimizations to see difference */

	/* areas to copy and move to */
	uint8_t *area_one = (uint8_t *) malloc(sizeof(uint8_t)*5020);
	uint8_t *area_two = area_one+20;
    uint8_t *retval = NULL;

	/* timer values */
	volatile uint32_t start_value = 0;
	volatile uint32_t end_value = 0;

	/* strings used for printing */
    uint8_t *my_string;
    uint8_t num_string[16];
	uint8_t printsize = 0x00;

	/*******************/
	/* MEMSET PROFILER */
	/*******************/

	/* standard library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memset library with 10 bytes took ";
	UART_send_n(my_string, 57);
	start_value = SysTick_Base_Ptr->CVR;
	memset(area_one, '1', 10);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 100 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memset library with 100 bytes took ";
	UART_send_n(my_string, 58);
	start_value = SysTick_Base_Ptr->CVR;
	memset(area_one, '2', 100);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 1000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memset library with 1000 bytes took ";
	UART_send_n(my_string, 59);
	start_value = SysTick_Base_Ptr->CVR;
	memset(area_one, '3', 1000);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 5000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memset library with 5000 bytes took ";
	UART_send_n(my_string, 59);
	start_value = SysTick_Base_Ptr->CVR;
	memset(area_one, '4', 5000);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* my library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memset function with 10 bytes took ";
	UART_send_n(my_string, 56);
	start_value = SysTick_Base_Ptr->CVR;
	my_memset(area_one, 10, '1');
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 100 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memset function with 100 bytes took ";
	UART_send_n(my_string, 57);
	start_value = SysTick_Base_Ptr->CVR;
	my_memset(area_one, 100, '2');
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 1000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memset function with 1000 bytes took ";
	UART_send_n(my_string, 58);
	start_value = SysTick_Base_Ptr->CVR;
	my_memset(area_one, 1000, '3');
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 5000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memset function with 5000 bytes took ";
	UART_send_n(my_string, 58);
	start_value = SysTick_Base_Ptr->CVR;
	my_memset(area_one, 5000, '4');
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* TODO the entire dma profiler */
	/* dma library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memset function with 10 bytes took ";
	UART_send_n(my_string, 53);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memset_dma(area_one, 10, '1', 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 100 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memset function with 100 bytes took ";
	UART_send_n(my_string, 54);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memset_dma(area_one, 100, '2', 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 1000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memset function with 1000 bytes took ";
	UART_send_n(my_string, 55);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memset_dma(area_one, 1000, '3', 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 5000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memset function with 5000 bytes took ";
	UART_send_n(my_string, 55);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memset_dma(area_one, 5000, '4', 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* dma library - 4 byte transfer */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memset function with 10 bytes and 4 byte transfer took ";
	UART_send_n(my_string, 73);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memset_dma(area_one, 10, '1', 4);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 100 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memset function with 100 bytes and 4 byte transfer took ";
	UART_send_n(my_string, 74);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memset_dma(area_one, 100, '2', 4);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 1000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memset function with 1000 bytes and 4 byte transfer took ";
	UART_send_n(my_string, 75);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memset_dma(area_one, 1000, '3', 4);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 5000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memset function with 5000 bytes and 4 byte transfer took ";
	UART_send_n(my_string, 75);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memset_dma(area_one, 5000, '4', 4);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);

	/********************/
	/* MEMMOVE PROFILER */
	/********************/
	/* standard library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memmove library with 10 bytes took ";
	UART_send_n(my_string, 58);
	start_value = SysTick_Base_Ptr->CVR;
	memmove(area_one, area_two, 10);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 100 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memmove library with 100 bytes took ";
	UART_send_n(my_string, 59);
	start_value = SysTick_Base_Ptr->CVR;
	memmove(area_one, area_two, 100);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 1000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memmove library with 1000 bytes took ";
	UART_send_n(my_string, 60);
	start_value = SysTick_Base_Ptr->CVR;
	memmove(area_one, area_two, 1000);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 5000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the standard memmove library with 5000 bytes took ";
	UART_send_n(my_string, 60);
	start_value = SysTick_Base_Ptr->CVR;
	memmove(area_one, area_two, 5000);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* my library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memmove function with 10 bytes took ";
	UART_send_n(my_string, 57);
	start_value = SysTick_Base_Ptr->CVR;
	my_memmove(area_one, area_two, 10);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 100 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memmove function with 100 bytes took ";
	UART_send_n(my_string, 58);
	start_value = SysTick_Base_Ptr->CVR;
	my_memmove(area_one, area_two, 100);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 1000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memmove function with 1000 bytes took ";
	UART_send_n(my_string, 59);
	start_value = SysTick_Base_Ptr->CVR;
	my_memmove(area_one, area_two, 1000);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 5000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the custom memsmovefunction with 5000 bytes took ";
	UART_send_n(my_string, 59);
	start_value = SysTick_Base_Ptr->CVR;
	my_memmove(area_one, area_two, 5000);
	end_value = SysTick_Base_Ptr->CVR;
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-end_value), num_string, 10);
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* dma library */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memmove function with 10 bytes took ";
	UART_send_n(my_string, 54);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memmove_dma(area_one, area_two, 10, 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 100 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memmove function with 100 bytes took ";
	UART_send_n(my_string, 55);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memmove_dma(area_one, area_two, 100, 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 1000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memmove function with 1000 bytes took ";
	UART_send_n(my_string, 56);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memmove_dma(area_one, area_two, 1000, 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 5000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memmove function with 5000 bytes took ";
	UART_send_n(my_string, 56);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memmove_dma(area_one, area_two, 5000, 1);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* dma library - 4 byte transfer */
	/* 10 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memmove function with 10 bytes and 4 byte transfer took ";
	UART_send_n(my_string, 74);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memmove_dma(area_one, area_two, 10, 4);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 100 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memmove function with 100 bytes and 4 byte transfer took ";
	UART_send_n(my_string, 75);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memmove_dma(area_one, area_two, 100, 4);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 1000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memmove function with 1000 bytes and 4 byte transfer took ";
	UART_send_n(my_string, 76);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memmove_dma(area_one, area_two, 1000, 4);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
	/* 5000 bytes */
	SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
    my_string = (unsigned char *) "Profiling the DMA memmove function with 5000 bytes and 4 byte transfer took ";
	UART_send_n(my_string, 76);
	start_value = SysTick_Base_Ptr->CVR;
	retval = memmove_dma(area_one, area_two, 5000, 4);
	SysTick_Base_Ptr->CVR = 0;
	SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
	printsize = my_itoa((int32_t)(start_value-DMA_end_value), num_string, 10);
    if(dma_error_flag)
    {
        dma_error_flag=0;
        num_string[0] = "-";
        num_string[1] = "1";
        printsize = 2;
    }
	UART_send_n(num_string, printsize);
    my_string = (unsigned char *) " clock cycles to run\r\n";
	UART_send_n(my_string, 22);
}
#endif
#ifdef BBB
void profiler()
{
	/* TODO buildtime settings must be adjusted so that the heap can support these regions (at least 1kB) */
	/* TODO refactor this code to be smaller */
	/* TODO run with highest optimizations and lowest optimizations to see difference */

	/* areas to copy and move to */
	uint8_t *area_one = (uint8_t *) malloc(sizeof(uint8_t)*5020);
	uint8_t *area_two = area_one+20;

	/* timer values */
	struct timespec start_value;
	struct timespec end_value;

	/*******************/
	/* MEMSET PROFILER */
	/*******************/

	/* standard library */
	/* 10 bytes */
    printf("Profiling the standard memset library with 10 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memset(area_one, '1', 10);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 100 bytes */
    printf("Profiling the standard memset library with 100 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memset(area_one, '2', 100);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 1000 bytes */
    printf("Profiling the standard memset library with 1000 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memset(area_one, '3', 1000);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 5000 bytes */
    printf("Profiling the standard memset library with 5000 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memset(area_one, '4', 5000);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* my library */
	/* 10 bytes */
    printf("Profiling the custom memset function with 10 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memset(area_one, 10, '1');
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 100 bytes */
    printf("Profiling the custom memset function with 100 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memset(area_one, 100, '2');
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 1000 bytes */
    printf("Profiling the custom memset function with 1000 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memset(area_one, 1000, '3');
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 5000 bytes */
    printf("Profiling the custom memset function with 5000 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memset(area_one, 5000, '4');
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");

	/********************/
	/* MEMMOVE PROFILER */
	/********************/
	/* standard library */
	/* 10 bytes */
    printf("Profiling the standard memmove library with 10 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memmove(area_one, area_two, 10);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 100 bytes */
    printf("Profiling the standard memmove library with 100 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memmove(area_one, area_two, 100);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 1000 bytes */
    printf("Profiling the standard memmove library with 1000 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memmove(area_one, area_two, 1000);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 5000 bytes */
    printf("Profiling the standard memmove library with 5000 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	memmove(area_one, area_two, 5000);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* my library */
	/* 10 bytes */
    printf("Profiling the custom memmove function with 10 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memmove(area_one, area_two, 10);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 100 bytes */
    printf("Profiling the custom memmove function with 100 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memmove(area_one, area_two, 100);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 1000 bytes */
    printf("Profiling the custom memmove function with 1000 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memmove(area_one, area_two, 1000);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
	/* 5000 bytes */
    printf("Profiling the custom memmove function with 5000 bytes took ");
	clock_gettime(CLOCK_MONOTONIC,&start_value);
	my_memmove(area_one, area_two, 5000);
	clock_gettime(CLOCK_MONOTONIC,&end_value);
	printf("%ld",(end_value.tv_nsec-start_value.tv_nsec));
    printf(" nanoseconds to run\r\n");
}
#endif

#ifdef KL25Z
void spi_setup_and_test()
{
    uint8_t my_string[] = "Starting project 3...";
    uint8_t return_string[] = "\r\n";
    uint8_t hex_string[] = "Status Reg is: 0x";
    UART_send_n(my_string, sizeof(my_string));
    UART_send_n(return_string, sizeof(return_string));
	uint8_t j = 0x00;
	uint8_t statreg = 0x00;
	uint8_t printsize = 0x00;
	nrf_write_register(0x05, j);
	statreg = nrf_read_register(0x05);
	printsize = my_itoa((int32_t)statreg, my_string, 16);
	UART_send_n(hex_string, sizeof(hex_string));
	UART_send_n(my_string, printsize);
	UART_send_n(return_string, sizeof(return_string));
}
#endif
#ifdef BBB
void spi_setup_and_test()
{
    printf("Starting project 3...\r\n");
	/*uint8_t j = 0x00;*/
	uint8_t statreg = 0x00;
	/*nrf_write_register(0x05, j);
	statreg = nrf_read_register(0x05);*/
	printf("Status Reg is: 0x");
	printf("%d\r\n",(int)statreg);
}
#endif
