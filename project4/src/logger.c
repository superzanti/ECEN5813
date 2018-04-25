 /**
 * @file logger.c
 * @brief implementation of logger.h
 *
 * this file implements logger.h, implementing blocking binary log functions for the
 * BBB and FRDM boards
 *
 * @author Seth Miers and Jake Cazden
 * @date April 29, 2018
 *
 */
#include "logger.h"

#include "circbuf.h" /* so we can use the circular buffer in our uart */

#if defined(BBB) || defined(HOST)
#include <stdio.h>
#include <time.h>
#endif

#ifdef KL25Z
#include "MKL25Z4.h"
#include "uart.h"
#endif

extern CB_t* log_buffer;

log_ret logger_init()
{
#ifdef BBB
#endif
#ifdef HOST
#endif
#ifdef KL25Z
	CB_e logbufferinitreturn = CB_init(log_buffer, LOG_BUFFER_LENGTH);
#endif
}

log_ret log_data(uint8_t* data, uint16_t length, mod_e module)
{
#if defined(BBB) || defined (HOST)
	time_t thetime = time(NULL);
	/*TODO change how the time is acquired on the BBB, it needs calibration*/
	uint8_t checksum = 0;
	char* timeptr = (char*)(&thetime);
	char* lengthptr = (char*)(&length);
	printf("%c",(char)INFO);/*print LOG ID*/
	checksum^=(uint8_t)INFO;
	printf("%c",(char)module);/*print module ID*/
	checksum^=(uint8_t)module;
	uint16_t i;
	for(i=0;i<2;i++)
	{
		checksum^=(uint8_t)(*lengthptr);
		printf("%c",(*(lengthptr++)));/*print length*/
	}
	for(i=0;i<4;i++)
	{
		checksum^=(uint8_t)(*timeptr);
		printf("%c",(*(timeptr++)));/*print time*/
	}
	for(i=0;i<length;i++)
	{
		checksum^=(uint8_t)(*data);
		printf("%c",(*((char*)data++)));/*print payload*/
	}
	printf("%c",(char)checksum);/*print checksum*/
	return LOGGER_SUCCESS;
#endif
#ifdef KL25Z
#endif
}

log_ret log_string(uint8_t* string, mod_e module)
{
#if defined(BBB) || defined (HOST)
	uint16_t i;
	for(i=0;i<65535;i++)
	{
		if(string[i]=='\0')
	}
#endif
#ifdef KL25Z
#endif
}

log_ret log_integer(uint32_t num, mod_e module)
{
#if defined(BBB) || defined (HOST)
#endif
#ifdef KL25Z
#endif
}

void log_flush()
{
#if defined(BBB) || defined (HOST)
#endif
#ifdef KL25Z
#endif
}

log_ret log_item(log_t loginput)
{
#if defined(BBB) || defined (HOST)
#endif
#ifdef KL25Z
#endif
}
