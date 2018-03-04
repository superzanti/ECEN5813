/**
 * @file port.h
 * @brief this file contains gpio and led port headers and functions
 *
 * this file contains headers for GPIO setup and accessing, primarily to
 * utilize onboard KL25z LEDs and enabling UART
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */

#ifndef __PORT_H__
#define __PORT_H__

#include <stdint.h>

/* which gpio pin is the LED on? */
/* These values are for the KL25Z */
#define RGB_RED_PIN (18)
#define RGB_GREEN_PIN (19)
#define RGB_BLUE_PIN (1)

/* Macro functions */
#define RGB_RED_ON() (PORTB_Set( RGB_RED_PIN ))
#define RGB_RED_OFF() (PORTB_Clear( RGB_RED_PIN ))
#define RGB_RED_TOGGLE() (PORTB_Toggle( RGB_RED_PIN ))
#define RGB_GREEN_ON() (PORTB_Set( RGB_GREEN_PIN ))
#define RGB_GREEN_OFF() (PORTB_Clear( RGB_GREEN_PIN ))
#define RGB_GREEN_TOGGLE() (PORTB_Toggle( RGB_GREEN_PIN ))
#define RGB_BLUE_ON() (PORTB_Set( RGB_BLUE_PIN ))
#define RGB_BLUE_OFF() (PORTB_Clear( RGB_BLUE_PIN ))
#define RGB_BLUE_TOGGLE() (PORTB_Toggle( RGB_BLUE_PIN ))


/**
 * @brief configures the RGB LEDs
 *
 * configures the gpios to be outputs for the
 * rgb leds. It then sets them to their default
 * values
 *
 * @param none
 * @return void returns nothing
 */
void inline GPIO_Configure();

/**
 * @brief toggles the red LED state
 *
 * turns the red LED on if it's off
 * turns the red LED off if it's on
 *
 * @param none
 * @return void returns nothing
 */
void inline Toggle_Red_LED();

/**
 * @brief sets an output to 1 (active high)
 *
 * @param the bit number to set on portB
 * @return void returns nothing
 */
void PORTB_Set(uint8_t bit_num);

/**
 * @brief sets an output to 1 (active high)
 *
 * @param the bit number to set on portD
 * @return void returns nothing
 */
void PORTD_Set(uint8_t bit_num);

/**
 * @brief sets an output to 0 (active low)
 *
 * @param the bit number to clear on portB
 * @return void returns nothing
 */
void PORTB_Clear(uint8_t bit_num);

/**
 * @brief sets an output to 0 (active low)
 *
 * @param the bit number to clear on portD
 * @return void returns nothing
 */
void PORTD_Clear(uint8_t bit_num);

/**
 * @brief toggles the output of the pin
 *
 * @param the bit number to toggle on portB
 * @return void returns nothing
 */
void inline PORTB_Toggle(uint8_t bit_num);

/**
 * @brief toggles the output of the pin
 *
 * @param the bit number to toggle on portD
 * @return void returns nothing
 */
void inline PORTD_Toggle(uint8_t bit_num);


#endif /*__PORT_H__*/
