/**
 * @file projec2.c
 * @brief implementation of projec2.h
 *
 * implements the data parsing code necessary for evaluating the UART
 * communication system for the KL25z
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */
#include<stdint.h>
#include"project2.h"
#include"circbuf.h"
#include<stdio.h>
#ifdef KL25Z
#include"uart.h"
#endif

extern CB_t* recieve_buffer;
extern CB_t* transmit_buffer;

void project2()
{
#ifdef KL25Z
    UART_configure();/*TODO put in better UART setup control*/
#endif
#ifdef HOST
    if(CB_init(recieve_buffer, CIRCBUF_HOST_LENGTH)!=SUCCESS);
    CB_init(transmit_buffer, CIRCBUF_HOST_LENGTH);
#endif
    if(recieve_buffer==NULL)return;
    if(transmit_buffer==NULL)return;
    uint8_t data=0;
    CB_e retval=SUCCESS;
#ifdef HOST
        uint8_t char_holder;
#endif
    while( data!=ASCII_OFFSET_EOF || data!=EOF || data!=0xff || data!='.')
    {
#ifdef HOST
	printf("Type a string to be processed, return to submit\n");
	do
	{
        	char_holder = (uint8_t)getchar();
        	if(CB_buffer_add_item(recieve_buffer,char_holder)!=SUCCESS)
		{
			printf("additem_failure\n");
			return;
		}
		else printf("additem_success\n");
		putchar(char_holder);
	}while(char_holder!='\n'||char_holder!='\r'||char_holder!=ASCII_OFFSET_EOF||char_holder!=EOF||data!=0xff);
#endif
#ifdef DEBUG
	printf("exiting_do_while_test\n");
#endif
        retval=CB_buffer_remove_item(recieve_buffer, &data);
        if(retval==SUCCESS)
        {
#ifdef HOST
	printf("removeitem_success, Data = %c\n",data);
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
#ifdef HOST
	else
	{
		printf("removeitem_failure\n");
	}
#endif
    }
    dump_statistics();
}

void dump_statistics()
{
    uint32_t i=0;

    uint8_t string1[] = "Statistics:\n";/*12 characters*/
    for(i=0;i<12;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(string1+i));
    }

    uint8_t string2[] = "\tAlphabetic Characters: ";/*24 characters*/
    uint8_t string_alphabetic[32] = {};
    uint32_t length = my_itoa(statistics.alphabetic,string_alphabetic,10);
    uint8_t stringnewline[] = " \n";
    for(i=0;i<24;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(string2+i));
    }
    for(i=0;i<length;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(string_alphabetic+i));
    }
    for(i=0;i<2;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(stringnewline+i));
    }

    uint8_t string3[] = "\tNumeric Characters: ";/*21 characters*/
    uint8_t string_numeric[32] = {};
    length = my_itoa(statistics.numeric,string_numeric,10);
    for(i=0;i<24;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(string3+i));
    }
    for(i=0;i<length;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(string_numeric+i));
    }
    for(i=0;i<2;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(stringnewline+i));
    }

    uint8_t string4[] = "\tPunctuation Characters: ";/*25 characters*/
    uint8_t string_punctuation[32] = {};
    length = my_itoa(statistics.punctuation,string_punctuation,10);
    for(i=0;i<25;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(string4+i));
    }
    for(i=0;i<length;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(string_punctuation+i));
    }
    for(i=0;i<2;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(stringnewline+i));
    }

    uint8_t string5[] = "\tMiscellaneous Characters: ";/*27 characters*/
    uint8_t string_miscellaneous[32] = {};
    length = my_itoa(statistics.miscellaneous,string_miscellaneous,10);
    for(i=0;i<25;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(string5+i));
    }
    for(i=0;i<length;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(string_miscellaneous+i));
    }
    for(i=0;i<2;i++)
    {
        CB_buffer_add_item(transmit_buffer,*(stringnewline+i));
    }

#ifdef KL25Z
    UART_start_buffered_transmission();
#endif
#ifdef HOST
    uint8_t char_to_print=0;
    while(CB_is_empty(transmit_buffer)!=EMPTY)
    {
        CB_buffer_remove_item(transmit_buffer, &char_to_print);
        printf("%c",char_to_print);
    }
#endif
}
