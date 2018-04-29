/*
 * @file project4.c
 * @brief implements project4.h
 *
 *  Project 4 functionality
 *
 *  @author Seth Miers and Jake Cazden
 *  @date April 26, 2018
 */

#include "project4.h"
#include "logger.h"
#include "logger_queue.h"
#include "circbuf.h"
#include<stdio.h>
#include<stdint.h>

#ifdef KL25Z
#include "uart.h"
#endif

extern CB_t* recieve_buffer;
extern LQ_t* log_buffer;

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
        my_string = (uint8_t*) "recieve buffer failure";
#ifdef LOGGING
        log_item((log_t){ERROR,FUNC_PROJECT4,22,0,my_string,0});
#endif
        return;
    }
    if(log_buffer==NULL)return;
#ifdef LOGGING
    log_item((log_t){SYSTEM_INITIALIZED,FUNC_PROJECT4,0,0,NULL,0});
#endif
    /*^^^ see  https://gcc.gnu.org/onlinedocs/gcc-4.3.2/gcc/Compound-Literals.html*/
    uint8_t data=0;
    CB_e retval=SUCCESS;
#ifdef HOST
    uint8_t char_holder=data;
    my_string = (unsigned char*) "Type a string to be processed, return to submit\n";
    log_item((log_t){INFO,FUNC_PROJECT4,48,0,my_string,0});
#endif
    while(1)
    {
    data=0;
    char_holder=0;
    while( data!=ASCII_OFFSET_EOF && data!=EOF && data!=0xff)
    {
#ifdef HOST
    do
    {
        if(data==ASCII_OFFSET_EOF||data==EOF)break;
        if(char_holder==ASCII_OFFSET_EOF||char_holder==EOF||data!=char_holder)break;
            char_holder = (uint8_t)getchar();
            CB_buffer_add_item(recieve_buffer,char_holder);
    }while(char_holder!='\n'&&char_holder!='\r'&&char_holder!=ASCII_OFFSET_EOF&&char_holder!=EOF&&char_holder!=0xff);
#endif
#ifdef LOGGING
        log_item((log_t){DATA_ANALYSIS_STARTED,FUNC_PROJECT4,0,0,NULL,0});
#endif
        retval=CB_buffer_remove_item(recieve_buffer, &data);
        if(retval==SUCCESS)
        {
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
    dump_statistics();
    }
}

void dump_statistics()
{
    uint8_t * my_string;
    uint8_t bufferstring[32] = {};
    uint8_t * stringnewline = (unsigned char*)"\r\n";
    my_string = (unsigned char*) "\r\nStatistics:";
    log_item((log_t){INFO,FUNC_PROJECT4,14,0,my_string,0});
    log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});

    my_string = (unsigned char*) "\tAlphabetic Characters: ";
    log_item((log_t){INFO,FUNC_PROJECT4,24,0,my_string,0});
    uint32_t length = my_itoa(statistics.alphabetic,bufferstring,10);
    log_item((log_t){INFO,FUNC_PROJECT4,length,0,bufferstring,0});
    log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});

    my_string = (unsigned char*) "\tNumeric Characters: ";
    log_item((log_t){INFO,FUNC_PROJECT4,21,0,my_string,0});
    length = my_itoa(statistics.numeric,bufferstring,10);
    log_item((log_t){INFO,FUNC_PROJECT4,length,0,bufferstring,0});
    log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});

    my_string = (unsigned char*) "\tPunctuation Characters: ";
    log_item((log_t){INFO,FUNC_PROJECT4,25,0,my_string,0});
    length = my_itoa(statistics.punctuation,bufferstring,10);
    log_item((log_t){INFO,FUNC_PROJECT4,length,0,bufferstring,0});
    log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});

    my_string = (unsigned char*) "\tMiscellaneous Characters: ";
    log_item((log_t){INFO,FUNC_PROJECT4,27,0,my_string,0});
    length = my_itoa(statistics.miscellaneous,bufferstring,10);
    log_item((log_t){INFO,FUNC_PROJECT4,length,0,bufferstring,0});
    log_item((log_t){INFO,FUNC_PROJECT4,2,0,stringnewline,0});

    log_flush();
}
