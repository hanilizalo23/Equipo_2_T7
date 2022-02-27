/**
	\file
	\brief
		This is a starter file to implement a function able to produce an accurate delay
		using the PIT module.
	\author PhD. José Luis Pizano Escalante
	\completed by Nelida Paulina Hernández Moya
	\date	19/02/2022
 */

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

gpio_pin_control_register_t pcr_gpioc_pin_6 = GPIO_MUX1|GPIO_PE|GPIO_PS; /**Switch 2*/
gpio_pin_control_register_t pcr_gpioa_pin_4 = GPIO_MUX1|GPIO_PE|GPIO_PS; /**Switch 3*/

gpio_pin_control_register_t input_intr_config = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;

int main(void)
{
	GPIO_clock_gating(GPIO_A); /**Clock of port A*/
	GPIO_clock_gating(GPIO_B); /**Clock of port B*/
	GPIO_clock_gating(GPIO_C); /**Clock of port C*/
	GPIO_clock_gating(GPIO_E); /**Clock of port E*/

	GPIO_pin_control_register(GPIO_B,bit_21,&pcr_gpiob_pin_21); /**Blue LED*/
	GPIO_pin_control_register(GPIO_B,bit_22,&pcr_gpiob_pin_22); /**Red LED*/
	GPIO_pin_control_register(GPIO_E,bit_26,&pcr_gpioe_pin_26); /**Green LED*/

	GPIO_pin_control_register(GPIO_C,bit_6,&pcr_gpioc_pin_6); /**Switch 2*/
	GPIO_pin_control_register(GPIO_A,bit_4,&pcr_gpioa_pin_4); /**Switch 3*/

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

	NVIC_set_basepri_threshold(PRIORITY_10);

	PIT_clock_gating(); /**Turning on the clock for the PIT**/
	PIT_enable(); /**Enabling of the PIT**/

	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_8); /**NVIC priority**/

	NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_5); /**Set priority and enabling the switch 2*/
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ,PRIORITY_5); /**Set priority and enabling the switch 3*/

	NVIC_global_enable_interrupts; /**Enabling NVIC*/

	PIT_delay(PIT_0, SYSTEM_CLOCK, DELAY); /**Function of the delay with the PIT*/

	GPIO_callback_init(GPIO_C, change_states); /**Callback for the switch 2*/
	GPIO_callback_init(GPIO_A, change_states); /**Callback for the switch 3*/

	while(1)
	{

	}
	return 0;
}
