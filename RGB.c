/**
       \file
       \brief
                    This is the source file for the colors needed in the main implementation.
       \author Mauricio Peralta Osorio
       \date 15/02/2022
*/

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include "GPIO.h"
#include "RGB.h"
#include "bits.h"
#include "Delay.h"

#define DELAY_CONST 600000 //To wait

void green(void) /**Turn on the green LED, waiting and turning it off*/
{
       GPIO_clear_pin(GPIO_E, bit_26);
       delay(DELAY_CONST);
       GPIO_set_pin(GPIO_E, bit_26);
}


void blue(void) /**Turn on the blue LED, waiting and turning it off*/
{
       GPIO_clear_pin(GPIO_B, bit_21);
       delay(DELAY_CONST);
       GPIO_set_pin(GPIO_B, bit_21);
}


void purple(void) /**Turn on the purple LED, waiting and turning it off*/
{
       GPIO_clear_pin(GPIO_B, bit_21);
       GPIO_clear_pin(GPIO_B, bit_22);
       delay(DELAY_CONST);
       GPIO_set_pin(GPIO_B, bit_21);
       GPIO_set_pin(GPIO_B, bit_22);
}

void red(void) /**Turn on the red LED, waiting and turning it off*/
{
       GPIO_clear_pin(GPIO_B, bit_22);
       delay(DELAY_CONST);
       GPIO_set_pin(GPIO_B, bit_22);
}

void yellow(void) /**Turn on the yellow LED, waiting and turning it off*/
{
       GPIO_clear_pin(GPIO_B, bit_22);
       GPIO_clear_pin(GPIO_E, bit_26);
       delay(DELAY_CONST);
       GPIO_set_pin(GPIO_B, bit_22);
       GPIO_set_pin(GPIO_E, bit_26);
}


void white(void) /**Turn on the white LED, waiting and turning it off*/
{
       GPIO_clear_pin(GPIO_B, bit_21);
       GPIO_clear_pin(GPIO_B, bit_22);
       GPIO_clear_pin(GPIO_E, bit_26);
       delay(DELAY_CONST);
       GPIO_set_pin(GPIO_B, bit_21);
       GPIO_set_pin(GPIO_B, bit_22);
       GPIO_set_pin(GPIO_E, bit_26);
}


