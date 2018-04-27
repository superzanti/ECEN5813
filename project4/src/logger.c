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

log_ret log_data(log_e log, mod_e module, uint16_t length, uint8_t* data)
{
#if defined(BBB) || defined (HOST)
	time_t thetime = time(NULL);
	uint8_t checksum = 0;
	char* timeptr = (char*)(&thetime);
	char* lengthptr = (char*)(&length);
	printf("%c",(char)log);/*print LOG ID*/
	checksum^=(uint8_t)log;
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
	UART_e UART_send_n(uint8_t *data, size_t num_bytes)
	time_t thetime = time(NULL);
	/*TODO get the time using RTC, not the above code*/
	uint8_t checksum = 0;
	checksum^=(uint8_t)log;
	checksum^=(uint8_t)module;/*calculate checksums over log and module ID*/
	uint8_t* timeptr = (uint8_t*)(&thetime);
	uint8_t* lengthptr = (uint8_t*)(&length);
	uint8_t* dataptr = (data);
	UART_send((uint8_t*)(&log));
	UART_send((uint8_t*)(&module));/*send log id and module id*/
	uint16_t i;
	for(i=0;i<2;i++)
	{
		checksum^=(uint8_t)(*(lengthptr++));/*calculate checksum over length*/
	}
	UART_send_n((uint8_t*)(&length),2);/*print length*/
	for(i=0;i<4;i++)
	{
		checksum^=(uint8_t)(*(timeptr++));/*calculate checksum over time*/
	}
	UART_send_n((uint8_t*)(&thetime),4);/*print time*/
	for(i=0;i<length;i++)
	{
		checksum^=(uint8_t)(*(dataptr++));/*calculate checksum over data*/
	}
	if(length>0) UART_send_n(data,length);/*print payload*/
	Uart_send(&checksum);/*print checksum*/
	return LOGGER_SUCCESS;
#endif
}

log_ret log_string(log_e log, mod_e module, uint8_t* string)
{
	uint16_t i;
	for(i=0;i<65535;i++)
	{
		if(string[i]=='\0')
		{
			break;
		}
	}
	if(i==65535) return LOGGER_FAILURE;
	return log_data(log, module, string, i+1);
}

log_ret log_integer(log_e log, mod_e module, uint32_t num)
{
	uint8_t outstring[16];
	uint8_t length = my_itoa(num, outstring, 10);
	return log_data(outstring,length,module);
}

void log_flush()
{
#if defined(BBB) || defined (HOST)
	return;/*there should never be anything *in* the buffer on BBB and HOST*/
#endif
#ifdef KL25Z
	/*TODO make this repeatedly check for the buffer being full, or maybe wait for
	 * a flag we can set at the end of the uart interrupt handler*/
#endif
}

log_ret log_item(log_t loginput)
{
#if defined(BBB) || defined (HOST)
	log_data(loginput.LogID, loginput.ModuleID, loginput.LogLength, loginput.PayloadData);
#endif
#ifdef KL25Z
	if(LQ_is_full(log_buffer)==LOGQUEUE_SUCCESS)/*if logger is not full*/
	{
		loginput.Timestamp = (uint32_t)time(NULL);
		/*TODO get the time accurately on the KL25z*/
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
}
