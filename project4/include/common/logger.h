/**
 * @file logger.h
 * @brief this file contains blocking binary log functions for use on the FRDM and BBB boards
 *
 * @author Seth Miers and Jake Cazden
 * @date April 29, 2018
 *
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdint.h> /* for uint8_t */

#include <stddef.h> /* for size_t */

/* what the circular buffer should initialize to *//*doubled this to fit tx message -JC*/
#define LOG_BUFFER_LENGTH               (1024)

#define  SIM_SOPT1_OSC32KSEL_1KLPO      (3)
#define  SIM_SOPT2_RTCCLKOUTSEL_CLEAR   (1)
#define  SIM_SCGC6_RTC_ENABLED          (1)

#define  RTC_CR_OSCE_ENABLED            (1)
#define  RTC_CR_UM_DISABLED             (0)
#define  RTC_CR_SUP_ENABLED             (1)
#define  RTC_CR_WPE_DISABLED            (0)
#define  RTC_CR_SWR_NORESET             (0)

#define  RTC_IER_TSIE_ENABLED           (1)
#define  RTC_IER_TAIE_DISABLED          (0)
#define  RTC_IER_TOIE_DISABLED          (0)
#define  RTC_IER_TIIE_DISABLED          (0)

#define RTC_SR_TCE_ENABLE               (1)

typedef enum {
    LOGGER_SUCCESS=0,
    LOGGER_FAILURE
} log_ret;

typedef enum {
    FUNC_CIRCBUF=0,
    FUNC_CONVERSION,
    FUNC_DATA,
    FUNC_DEBUG,
    FUNC_LOGGER,
    FUNC_LOGGER_QUEUE,
    FUNC_MAIN,
    FUNC_MEMORY,
    FUNC_NORDIC,
    FUNC_PORT,
    FUNC_PROJECT1,
    FUNC_PROJECT2,
    FUNC_PROJECT3,
    FUNC_PROJECT4,
    FUNC_SPI,
    FUNC_UART,
    FUNC_UNITTEST
} mod_e;

typedef enum {
    SYSTEM_ID=0,
    SYSTEM_VERSION,
    LOGGER_INITIALIZED,
    GPIO_INITIALIZED,
    SYSTEM_INITIALIZED,
    SYSTEM_HALTED,
    INFO,
    WARNING,
    ERROR,
    PROFILING_STARTED,
    PROFILING_RESULT,
    PROFILING_COMPLETED,
    DATA_RECIEVED,
    DATA_ANALYSIS_STARTED,
    DATA_ALPHA_COUNT,
    DATA_NUMERIC_COUNT,
    DATA_PUNCTUATION_COUNT,
    DATA_MISC_COUNT,
    DATA_ANALYSIS_COMPLETED,
    HEARTBEAT,
    CORE_DUMP
} log_e;

typedef struct{
    log_e LogID;
    mod_e ModuleID;
    uint16_t LogLength;
    uint32_t Timestamp;
    uint8_t* PayloadData;
    uint8_t Checksum;
}log_t;

/* @brief initialize the logger engine
 *
 * @param none
 * @return success or failure of logger system initialization.
 */
log_ret logger_init();

/* @brief log an arbitrary datatype as a byte array and length
 *
 * @param[in] uint8_t* a pointer to the data to be logged
 * @param[in] uint16_t the length of the data, no larger than 2^16-1 bytes
 * @return returns success or failure of logging attempt
 */
log_ret log_data(log_e log, mod_e module, uint16_t length, uint8_t* data);

/* @brief log a c style string, relies on null termination.
 *
 * @param[in] uint8_t* a pointer to the c string to be logged
 * @return returns success or failure of logging attempt
 */
log_ret log_string(log_e log, mod_e module, uint8_t* string);

/* @brief log a numeric type cast as a uint32_t. 
 *
 * this function converts the input number into a byte array, then logs that
 *
 * @param[in] uint32_t a cast version of any 32 bit or smaller numeric type
 * @return returns success or failure of logging attempt
 */
log_ret log_integer(log_e log, mod_e module, uint32_t num);

/* @brief blocks until the log buffer is empty
 *
 * @param none
 * @return none, returns void
 */
void log_flush();

/* @brief pushes an item into the logging queue. does not directly log data
 *
 * @param[in] log_t* a pointer to a log type to be pushed into the queue
 * @return returns success or failure of loading queue with log data
 */
log_ret log_item(log_t loginput);

#ifdef KL25Z
/*@brief acts as heartbeat timer interrupt
 *
 * @param none
 * @return none
 * */
void RTC_Seconds_IRQHandler();
#endif
#endif /*__LOGGER_H__*/
