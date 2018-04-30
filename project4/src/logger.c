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
FILE* logfile=NULL;
#endif

#ifdef KL25Z
#include "MKL25Z4.h"
#include "uart.h"
#endif

#ifdef PROJECT4
extern LQ_t* log_buffer;
#endif
extern uint32_t nooperation;

uint32_t time_orig = 0;

log_ret logger_init()
{
    uint32_t currenttime;
    uint8_t* currenttime_byte = (uint8_t*)&currenttime;
    UART_recieve((uint8_t*)(currenttime_byte++));
    UART_recieve((uint8_t*)(currenttime_byte++));
    UART_recieve((uint8_t*)(currenttime_byte++));
    UART_recieve((uint8_t*)(currenttime_byte));
    time_orig = currenttime;
    #ifdef PROJECT4
    LQ_e logbufferinitreturn = LQ_init(log_buffer, LOG_BUFFER_LENGTH);
    if(logbufferinitreturn!=LOGQUEUE_SUCCESS)
    {
        return LOGGER_FAILURE;
    }
    #endif
#if defined(BBB) || defined(HOST)
    logfile = fopen("Log_output.txt","a+");
    if(logfile==NULL)
    {
        return LOGGER_FAILURE;
    }
    /*nothing else is needed unless we want to set up the clock on the BBB*/
#endif
#ifdef KL25Z/*if we're on the KL25z, then turn on the RTC*/
    /*sim_sopt1, osc32sel = 00, set rtc to use 32khz onboard oscillator*/
    /*sim_sopt2, rtcclkoutsel = 0 or 1, largely irrelevent for our use*/
    /*sim_scgc6, rtc = 1, enable clocking and interrupts for rtc*/
    /*set up RTC_CR register - enable OSC and non-supervisor writes, etc*/
    /*sleep 2 seconds for oscillator to calm down*/
    /*block on waiting for program - get current time*/
    /*set RTC_TSR register - set the current time*/
    /*set up RTC_IER register - set up interrutps*/
    /*set up RTC_SR register - enable counting*/
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(SIM_SOPT1_OSC32KSEL_1KLPO);/*set bits to 00*/
    SIM_SOPT2 &= ~SIM_SOPT2_RTCCLKOUTSEL(SIM_SOPT2_RTCCLKOUTSEL_CLEAR);/*set bits to 0*/
    SIM_SCGC6 |= SIM_SCGC6_RTC(SIM_SCGC6_RTC_ENABLED);/*set bits to 1 to enable clock gate*/

    RTC_CR =    RTC_CR_OSCE(RTC_CR_OSCE_ENABLED)| RTC_CR_UM(RTC_CR_UM_DISABLED)|
        RTC_CR_SUP(RTC_CR_SUP_ENABLED)| RTC_CR_WPE(RTC_CR_WPE_DISABLED)|
        RTC_CR_SWR(RTC_CR_SWR_NORESET);
    /*i looked around and there's no sleep function in C by default*/

    RTC_IER = RTC_IER_TSIE(RTC_IER_TSIE_ENABLED) | RTC_IER_TAIE(RTC_IER_TAIE_DISABLED) |
            RTC_IER_TOIE(RTC_IER_TOIE_DISABLED) | RTC_IER_TIIE(RTC_IER_TIIE_DISABLED);

    RTC_SR = RTC_SR_TCE(RTC_SR_TCE_ENABLE);

    RTC_SR &= ~RTC_SR_TCE_MASK;
    RTC_TSR = currenttime; //Reset counter
    RTC_SR |= RTC_SR_TCE_MASK;

    NVIC_ClearPendingIRQ(RTC_Seconds_IRQn);
    NVIC_EnableIRQ(RTC_Seconds_IRQn);
#endif
#ifdef LOGGING
    log_item((log_t) {LOGGER_INITIALIZED,FUNC_LOGGER,0,0,NULL,0});
#endif
    return LOGGER_SUCCESS;
}

log_ret log_data(log_e log, mod_e module, uint16_t length, uint8_t* data)
{
#if defined(BBB) || defined (HOST)
    time_t thetime = time(NULL);
    uint8_t checksum = 0;
    char* timeptr = (char*)(&thetime);
    char* lengthptr = (char*)(&length);
    if(log!=INFO) printf("%c",(char)log);/*print LOG ID*/
    fprintf(logfile,"%c",(char)log);/*print LOG ID*/
    checksum^=(uint8_t)log;
    if(log!=INFO) printf("%c",(char)module);/*print module ID*/
    fprintf(logfile,"%c",(char)module);/*print module ID*/
    checksum^=(uint8_t)module;
    uint16_t i;
    for(i=0;i<2;i++)
    {
        checksum^=(uint8_t)(*lengthptr);
        if(log!=INFO) printf("%c",(char)(*(lengthptr)));/*print length*/
        fprintf(logfile,"%c",(char)(*(lengthptr)));/*print length*/
        lengthptr++;
    }
    for(i=0;i<4;i++)
    {
        checksum^=(uint8_t)(*timeptr);
        if(log!=INFO) printf("%c",(char)(*(timeptr)));/*print time*/
        fprintf(logfile,"%c",(char)(*(timeptr)));/*print time*/
        timeptr++;
    }
    for(i=0;i<length;i++)
    {
        checksum^=(uint8_t)(*data);
        printf("%c",(*((char*)data)));/*print payload*/
        fprintf(logfile,"%c",(*((char*)data)));/*print payload*/
        data++;
    }
    if(log!=INFO) printf("%c",(char)checksum);/*print checksum*/
    fprintf(logfile,"%c",(char)checksum);/*print checksum*/
    fflush(logfile);
    return LOGGER_SUCCESS;
#endif
#ifdef KL25Z
    uint32_t thetime = RTC_TSR+(RTC_TSR-time_orig)*30+((RTC_TPR>>10)&0x1F);
    /*uint32_t thetime = RTC_TSR;*/
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
    UART_send(&checksum);/*print checksum*/
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
    return log_data(log, module, i+1, string);
}

log_ret log_integer(log_e log, mod_e module, uint32_t num)
{
    uint8_t outstring[16];
    uint8_t length = my_itoa(num, outstring, 10);
    return log_data(log, module, length, outstring);
}

void log_flush()
{
#if defined(BBB) || defined (HOST)
    return;/*there should never be anything *in* the buffer on BBB and HOST*/
#endif
#ifdef KL25Z
    UART_start_buffered_transmission();/*do we need this here?*/
    while(LQ_is_empty(log_buffer)!=LOGQUEUE_EMPTY)
    {
        nooperation++;
    }
    /*TODO make this repeatedly check for the buffer being full, or maybe wait for
     * a flag we can set at the end of the uart interrupt handler*/
    return;
#endif
}

log_ret log_item(log_t loginput)
{
#if defined(BBB) || defined (HOST)
    return log_data(loginput.LogID, loginput.ModuleID, loginput.LogLength, loginput.PayloadData);
#endif
#ifdef KL25Z
    if(LQ_is_full(log_buffer)==LOGQUEUE_SUCCESS)/*if logger is not full*/
    {
        loginput.Timestamp = RTC_TSR+(RTC_TSR-time_orig)*30+((RTC_TPR>>10)&0x1F);
        /*loginput.Timestamp = (uint32_t)(RTC_TSR);*/
        loginput.Checksum = 0;
        loginput.Checksum^=(uint8_t)loginput.LogID;
        loginput.Checksum^=(uint8_t)loginput.ModuleID;
        uint8_t* timeptr = (uint8_t*)(&loginput.Timestamp);
        uint8_t* lengthptr = (uint8_t*)(&loginput.LogLength);
        uint8_t* dataptr = (loginput.PayloadData);
        uint16_t i;
        for(i=0;i<2;i++)
        {
            loginput.Checksum^=(*(lengthptr++));
        }
        for(i=0;i<4;i++)
        {
            loginput.Checksum^=(*(timeptr++));
        }
        for(i=0;i<loginput.LogLength;i++)
        {
            loginput.Checksum^=(*(dataptr++));
        }
        LQ_buffer_add_item(log_buffer, &loginput);
        return LOGGER_SUCCESS;
    }
    return LOGGER_FAILURE;
#endif
}

#ifdef KL25Z
void RTC_Seconds_IRQHandler()
{
    /*TODO implement this. NB: the irq doesn't need to be cleared*/
#ifdef LOGGING
    log_item((log_t) {HEARTBEAT,FUNC_LOGGER,0,0,NULL,0});
#endif
}
#endif
