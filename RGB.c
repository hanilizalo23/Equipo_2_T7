/**
       \file
       \brief
                    This is the source file for the colors needed in the main implementation.
       \author Nelida Paulina Hernández Moya
       \date 15/02/2022
*/

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include "GPIO.h"
#include "RGB.h"
#include "bits.h"
#include "PIT.h"

typedef struct /**Struct that contanis the elements of every state, that is the sequence to run and the next state*/
{
	void(*fptrSequence)(void);
	uint8_t next[4];
}State_t;

/**States of the machine*/
const State_t FSM_Moore[4] =
{
		{no_color, {sequence01, sequence10, sequence11, sequence00}},
		{y_r_p,    {sequence10, sequence11, sequence00, sequence10}},
		{g_r_w,    {sequence11, sequence00, sequence01, sequence10}},
		{b_g_w,    {sequence00, sequence01, sequence10, sequence11}}
};

uint8_t current_state = sequence00; /**Actual state*/
uint32_t input; /**Next state will be determined by this variable*/

void change_states(void) /**Comparing the inputs and assigning the correspondent state*/
{
	if(((GPIOA->PDIR & 0x00000010) == 0x10) && ((GPIOC->PDIR & 0x00000040) == 0x40))
	{
		input = sequence00;
	}
	else if(((GPIOA->PDIR & 0x00000010) == 0x10) && ((GPIOC->PDIR & 0x00000040) == 0x0))
	{
		input = sequence01;
	}
	else if(((GPIOA->PDIR & 0x00000010) == 0x0) && ((GPIOC->PDIR & 0x00000040) == 0x40))
	{
		input = sequence10;
	}
	else if(((GPIOA->PDIR & 0x00000010) == 0x0) && ((GPIOC->PDIR & 0x00000040) == 0x0))
	{
		input = sequence11;
	}

	FSM_Moore[current_state].fptrSequence();

	current_state = FSM_Moore[current_state].next[input];
}

void y_r_p(void) /**Sequence of yellow, red and purple with a delay of 2 seconds with the PIT*/
{
	yellow();
	red();
	purple();
}

void g_r_w(void) /**Sequence of green, red and white with a delay of 2 seconds with the PIT*/
{
	green();
	red();
	white();
}

void b_g_w(void) /**Sequence of purple, green and white with a delay of 2 seconds with the PIT*/
{
	blue();
	green();
	white();
}

void green(void) /**Turn on the green LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_E, bit_26);
	PIT_loop();
	GPIO_set_pin(GPIO_E, bit_26);

	PIT_clear_interrupt_flag();
}

void blue(void) /**Turn on the blue LED, waiting and turning it off*/
{
    GPIO_clear_pin(GPIO_B, bit_21);
	PIT_loop();
	GPIO_set_pin(GPIO_B, bit_21);

	PIT_clear_interrupt_flag();
}

void purple(void) /**Turn on the purple LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	PIT_loop();
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);

   	PIT_clear_interrupt_flag();
}

void red(void) /**Turn on the red LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_22);
	PIT_loop();
	GPIO_set_pin(GPIO_B, bit_22);

	PIT_clear_interrupt_flag();
}

void yellow(void) /**Turn on the yellow LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	PIT_loop();
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);

	PIT_clear_interrupt_flag();
}

void white(void) /**Turn on the white LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	PIT_loop();
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);

	PIT_clear_interrupt_flag();
}

void no_color(void) /**Turn on none color*/
{
	PIT_loop();

	PIT_clear_interrupt_flag();
}
