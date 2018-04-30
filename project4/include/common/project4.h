/*
 * @file project4.h
 * @brief
 *
 *
 *
 *  @author Seth Miers and Jake Cazden
 *  @date April 26, 2018
 */

#ifndef __PROJECT4_H_
#define __PROJECT4_H_

#include<stdint.h>
#include"conversion.h"
typedef struct{
    uint32_t alphabetic;
    uint32_t numeric;
    uint32_t punctuation;
    uint32_t miscellaneous;
}charcounts;
volatile charcounts statistics;
#define CIRCBUF_HOST_LENGTH (256)

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
#define ASCII_OFFSET_EOF  (0x4)
#endif

/*
 * @brief tests logging functionality using
 *          the code from project 2 and 3
 *
 * @param none
 * @return void returns nothing
 *
 */
void project4();

void project4_dump_statistics();
void project4_profiler();
#endif /* __PROJECT4_H_ */
