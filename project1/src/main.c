/**
 * @file main.c
 * @brief calls test functionality for project 1 on multiple platorms
 *
 * this main file suports calling test functions on the project 1
 * function implementations. This will be expanded later to include
 * more functionality
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2017
 *
 */
#ifdef PROJECT1
    #include "project1.h"
#endif
int main(void)
{
    #ifdef PROJECT1
        project1();
    #endif
    return 0;
}
