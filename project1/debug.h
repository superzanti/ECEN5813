#ifndef __DEBUG_H__
#define __DEBUG_H__

/*
 * @brief function to print the bytes in memory starting at an address
 * 
 * This function takes in a pointer to the start of some
 * memory region and starts pringing out hex bytes
 * for the number of bytes specified.
 *
 * @param start a pointer to the start of the memory region
 * @param length the number of bytes to read back
 * @return void don't return anything
 */
void print_array(void * start, uint32_t length);

#endif /* __DEBUG_H__*/
