/**
	\file
	\brief
		This is a starter file to implement a function able to produce an accurate delay
		using the PIT module.
	\author PhD. José Luis Pizano Escalante
	\completed by Nelida Paulina Hernández Moya
	\date	19/02/2022
 */

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include "PIT.h"
#include "NVIC.h"
#include "GPIO.h"
#include "Bits.h"
#include "RGB.h"

#define SYSTEM_CLOCK (21000000U) /** Maximum frequency of the K64*/
#define DELAY (2.0F) /**Required delay for the sequence*/

#define GPIO_OFF_CONST (0xFFFFFFFFU) /**To turn off the ports*/
#define GPIO_ON_CONST (0U) /**To turn on the ports*/

gpio_pin_control_register_t pcr_gpiob_pin_21 = GPIO_MUX1; /**Blue LED*/
gpio_pin_control_register_t pcr_gpiob_pin_22 = GPIO_MUX1; /**Red LED*/
gpio_pin_control_register_t pcr_gpioe_pin_26 = GPIO_MUX1; /**Green LED*/

gpio_pin_control_register_t input_intr_config = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE; /**For interruptions configuration*/

typedef enum {sequence00, sequence01, sequence10, sequence11} sequences; /**Enum for the next states*/

typedef struct /**Struct that contanis the elements of every state, that is the sequence to run and the next state*/
{
	uint8_t (*p[3]) (void);
	uint8_t next[4];
}State_t;

/**States of the machine*/
const State_t FSM_Moore[4] =
{
		{ {no_color, no_color, no_color}, {sequence00, sequence01, sequence10, sequence11} },
		{ {yellow, red, purple},          {sequence00, sequence01, sequence10, sequence11} },
		{ {green, red, white},            {sequence00, sequence01, sequence10, sequence11} },
		{ {blue, green, white},           {sequence00, sequence01, sequence10, sequence11} }
};

int main(void)
{
	GPIO_clock_gating(GPIO_A); /**Clock of port A*/
	GPIO_clock_gating(GPIO_B); /**Clock of port B*/
	GPIO_clock_gating(GPIO_C); /**Clock of port C*/
	GPIO_clock_gating(GPIO_E); /**Clock of port E*/

	GPIO_pin_control_register(GPIO_B,bit_21,&pcr_gpiob_pin_21); /**Blue LED*/
	GPIO_pin_control_register(GPIO_B,bit_22,&pcr_gpiob_pin_22); /**Red LED*/
	GPIO_pin_control_register(GPIO_E,bit_26,&pcr_gpioe_pin_26); /**Green LED*/

	GPIO_write_port(GPIO_B, GPIO_OFF_CONST); /**Turn off the port of the blue and red LED*/
	GPIO_write_port(GPIO_E, GPIO_OFF_CONST); /**Turn off the port of the green LED*/

	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT, bit_21); /**Blue LED*/
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT,bit_22); /**Red LED*/
	GPIO_data_direction_pin(GPIO_E,GPIO_OUTPUT,bit_26); /**Green LED*/

	GPIO_data_direction_pin(GPIO_C,GPIO_INPUT, bit_6); /**Switch 2*/
	GPIO_data_direction_pin(GPIO_A,GPIO_INPUT, bit_4); /**Switch 3*/

	/**Turning on and off the PDOR ports of the LEDs, part of the configuration*/
	GPIO_write_port(GPIO_B, GPIO_ON_CONST);
	GPIO_write_port(GPIO_B, GPIO_OFF_CONST);
	GPIO_write_port(GPIO_E, GPIO_ON_CONST);
	GPIO_write_port(GPIO_E, GPIO_OFF_CONST);

	GPIO_pin_control_register(GPIO_C, bit_6, &input_intr_config); /**Switch 2 as an interrupt*/
	GPIO_pin_control_register(GPIO_A, bit_4, &input_intr_config); /**Switch 3 as an interrupt*/

	NVIC_set_basepri_threshold(PRIORITY_10); /**Limit for the priorities*/

	PIT_clock_gating(); /**Turning on the clock for the PIT**/
	PIT_enable(); /**Enabling of the PIT**/

	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_8); /**NVIC priority**/
	PIT_delay(PIT_0, SYSTEM_CLOCK, DELAY); /**Function of the delay with the PIT*/

	NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_4); /**Set priority and enabling the switch 2*/
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ,PRIORITY_3); /**Set priority and enabling the switch 3*/

	GPIO_callback_init(GPIO_C, SW2_cb); /**Callback for the switch 2*/
	GPIO_callback_init(GPIO_A, SW3_cb); /**Callback for the switch 3*/

	PIT_callback_init(PIT_cb); /**Callback for the PIT*/

	NVIC_global_enable_interrupts; /**Enabling NVIC*/

	uint8_t current_state = 0; /**Actual state*/
	uint8_t input = 0; /**Next state will be determined by this variable*/
	uint8_t (*output[3]) (void); /**Array of pointers to function for asign the color of the sequence that follows*/
	uint8_t color_seq = 0; /**Variable for saving the sequence that corresponds*/

	while(1)
	{
		/**Assigning the values of the pointers to the variables*/
		output[sequence00] = FSM_Moore[current_state].p[sequence00];
		output[sequence01] = FSM_Moore[current_state].p[sequence01];
		output[sequence10] = FSM_Moore[current_state].p[sequence10];

		/**Getting the color*/
		color_seq = get_color();

		/**Putting the color that follows into the array as the index*/
		output[color_seq]();

		/**Getting the sequence*/
		input = get_sequence();

		/**Next state that is determined for the actual input of the system*/
		current_state = FSM_Moore[current_state].next[input];
	}
	return 0;
}
