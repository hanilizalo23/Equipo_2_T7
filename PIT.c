/*
* PIT.c
*
*  Created on: 19 feb 2022
*      Author: Mauricio Peralta Osorio
*/

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include "PIT.h"
#include "GPIO.h"
#include "Bits.h"

uint8_t g_flag_PIT_0 = FALSE; /** Global flag for the interrupt*/

void PIT0_IRQHandler(void) /** ISR of the PIT_0*/
{
     uint32_t dummyRead;
     PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK; /** Clears the flag*/
     dummyRead = PIT->CHANNEL[0].TCTRL; /** Read control register for clear PIT flag, this is silicon bug*/
     g_flag_PIT_0 = TRUE; /** Clears the programmer flag*/
}

void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock, My_float_pit_t delay)
{
	float LDVALUE = 0.0F;
	float clock_time = 0.0F;
	system_clock = system_clock /2;
	clock_time = (1/system_clock);
	LDVALUE = (delay / clock_time);
	LDVALUE = LDVALUE - 1;

     switch(pit_timer) /** PIT to choose*/
           {
           case PIT_0:/** PIT 0 is selected*/
                PIT->CHANNEL[0].LDVAL = LDVALUE;
                PIT_enable_interrupt(PIT_0);
                break;
           case PIT_1:/** PIT 1 is selected*/
        	   	PIT->CHANNEL[1].LDVAL = LDVALUE;
                PIT_enable_interrupt(PIT_1);
                break;
           case PIT_2:/** PIT 2 is selected*/
        	   	PIT->CHANNEL[2].LDVAL = LDVALUE;
                PIT_enable_interrupt(PIT_2);
                break;
           case PIT_3:/** PIT 3 is selected*/
        	    PIT->CHANNEL[3].LDVAL = LDVALUE;
                PIT_enable_interrupt(PIT_3);
                break;
           default:/**If doesn't exist the option do nothing*/
           break;
           }
}

void PIT_clock_gating(void) /**Turn on the clock for the PIT*/
{
     SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

uint8_t PIT_get_interrupt_flag_status(void) /** Returns the state of the application flag*/
{
     return (g_flag_PIT_0);
}

void PIT_clear_interrupt_flag(void) /** Cleans the application flag*/
{
     g_flag_PIT_0 = 0;
}

void PIT_enable(void) /** Enables the controls and timer clocks for the PITs*/
{
     PIT->MCR |= ~PIT_MCR_FRZ_MASK;
     PIT->MCR &= ~PIT_MCR_MDIS_MASK;
}

void PIT_enable_interrupt(PIT_timer_t pit) /** Configures the timers for the PIT*/
{
     PIT->CHANNEL[pit].TCTRL |= PIT_TCTRL_TIE_MASK;
     PIT->CHANNEL[pit].TCTRL |= PIT_TCTRL_TEN_MASK;
}
