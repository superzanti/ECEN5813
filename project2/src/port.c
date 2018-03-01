/**
 * @file port.c
 * @brief implementation of port.h
 *
 * this file contains implementations of GPIO setup and accessing code
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */

#include"port.h"
#include "MKL25Z4.h"

__attribute((always_inline))
void GPIO_Configure()
{
    /* clock gating */
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

    /* set to use GPIO mode */
    PORTB_PCR18 |= PORT_PCR_MUX(1);
    PORTB_PCR19 |= PORT_PCR_MUX(1);
    PORTD_PCR1 |= PORT_PCR_MUX(1);

    /* set to outputs */
    GPIOB_PDDR |= ((1 << RGB_RED_PIN) & (1 << RGB_GREEN_PIN));
    GPIOD_PDDR |= (1 << RGB_BLUE_PIN);

    /* set to logical low */
    GPIOB_PCOR |= ((1 << RGB_RED_PIN) & (1 << RGB_GREEN_PIN));
    GPIOD_PCOR |= (1 << RGB_BLUE_PIN);

}

__attribute((always_inline))
void Toggle_Red_LED()
{
    GPIOB_PTOR |= (1 << RGB_RED_PIN);
}

__attribute((always_inline))
void PORTB_Set(uint8_t bit_num)
{
    GPIOB_PSOR |= (1 << bit_num);
}

__attribute((always_inline))
void PORTD_Set(uint8_t bit_num)
{
    GPIOD_PSOR |= (1 << bit_num);
}

__attribute((always_inline))
void PORTB_Clear(uint8_t bit_num)
{
    GPIOB_PCOR |= (1 << bit_num);
}

__attribute((always_inline))
void PORTD_Clear(uint8_t bit_num)
{
    GPIOD_PCOR |= (1 << bit_num);
}

__attribute((always_inline))
void PORTB_Toggle(uint8_t bit_num)
{
    GPIOB_PTOR |= (1 << bit_num);
}

__attribute((always_inline))
void PORTD_Toggle(uint8_t bit_num)
{
    GPIOD_PTOR |= (1 << bit_num);
}
