/**
 * @file projec2.h
 * @brief contains function headers for simple data processing of strings
 *
 * this file contains the function headers for performing data parsing on
 * strings recieved via UART on the kl25z system
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */

#ifndef __PROJECT2_H__
#define __PROJECT2_H__

#include"conversion.h"
typedef struct{
    uint32_t alphabetic;
    uint32_t numeric;
    uint32_t punctuation;
    uint32_t miscellaneous;
}charcounts;
volatile charcounts statistics;
#ifndef __ASCII_OFFSETS__
#define __ASCII_OFFSETS__
#define ASCII_OFFSET_0  (48)
#define ASCII_OFFSET_9  (57)
#define ASCII_OFFSET_A  (65)
#define ASCII_OFFSET_Z  (90)
#define ASCII_OFFSET_A_ADDITION  (55)/*10+55= 'A' for hex interpreting*/
#define ASCII_OFFSET_F  (70)
#define ASCII_OFFSET_LA  (97)
#define ASCII_OFFSET_LZ  (122)
#define ASCII_OFFSET_LA_ADDITION  (87)/*10+87= 'a' for hex interpreting*/
#define ASCII_OFFSET_LF  (102)
#endif

/*
 *@brief interprets a datastream recieved over UART, finishes on EOF
 *
 *
 *
 *
 *
 *
 * */

void project2();
/*
 *@brief interprets a datastream recieved over UART
 *
 *
 *
 *
 *
 *
 * */

void dump_statistics();
#endif /*__PROJECT2_H__*/
