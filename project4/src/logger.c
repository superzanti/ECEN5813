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
#include "conversion.h"

#include "logger_queue.h" /* TODO fix circular dependency*/
#include "circbuf.h" /* so we can use the circular buffer in our uart */

#if defined(BBB) || defined(HOST)
#include <stdio.h>
#include <time.h>
#endif

#ifdef KL25Z
#include "MKL25Z4.h"
#include "uart.h"
#endif

extern LQ_t* log_buffer;

log_ret logger_init()
{
	LQ_e logbufferinitreturn = LQ_init(log_buffer, LOG_BUFFER_LENGTH);
#if defined(BBB) || defined(HOST)
#endif
#ifdef KL25Z
	/*TODO put in the RTC initialization here*/
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
		{
			break;
		}
	}
	return log_data(string,i+1,module);
#endif
#ifdef KL25Z
#endif
}

log_ret log_integer(uint32_t num, mod_e module)
{
#if defined(BBB) || defined (HOST)
	uint8_t outstring[16];
	uint8_t length = my_itoa(num, outstring, 10);
	return log_data(outstring,length,module);
#endif
#ifdef KL25Z
#endif
}

void log_flush()
{
/*TODO do we want this to go through and print each one? or do we want it to pop them off
 * the queue, and have the function in logger_queue do the printing over there*/
#if defined(BBB) || defined (HOST)
#endif
#ifdef KL25Z
#endif
}

log_ret log_item(log_t loginput)
{
#if defined(BBB) || defined (HOST)
	if(LQ_is_full(log_buffer)==LOGQUEUE_SUCCESS)/*if logger is not full*/
	{
		loginput.Timestamp = (uint32_t)time(NULL);
		/*TODO change how the time is acquired on the BBB, it needs calibration*/
		loginput.Checksum = 0;
		Loginput.Checksum^=(uint8_t)LogID;
		Loginput.Checksum^=(uint8_t)loginput.ModuleID;
		uint8_t* timeptr = (uint8_t*)(&loginput.Timestamp);
		uint8_t* lengthptr = (uint8_t*)(&LogLength);
		uint8_t* dataptr = (PayloadData);
		uint16_t i;
		for(i=0;i<2;i++)
		{
			Loginput.Checksum^=(*(lengthptr++));
		}
		for(i=0;i<4;i++)
		{
			Loginput.Checksum^=(*(timeptr++));
		}
		for(i=0;i<length;i++)
		{
			Loginput.Checksum^=(*(dataptr++));
		}
		LQ_e LQ_buffer_add_item(log_buffer, loginpu);
		return LOGGER_SUCCESS;
	}
#endif
#ifdef KL25Z
#endif
}
