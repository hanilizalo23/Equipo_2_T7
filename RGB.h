/**
	\file
	\brief
			This is the source file for the colors needed in the main implementation.
	\author Nelida Paulina Hernández Moya
	\date	15/02/2022
 */

#ifndef RGB_H_
#define RGB_H_

typedef enum {sequence00, sequence01, sequence10, sequence11} sequences; /**Enum for the next states*/

void change_states(void); /*Function for changing the states*/

/**Kind of a cheat, but is easier in this way*/
void y_r_p(void); /**Function with the colors yellow, red and purple*/
void g_r_w(void); /**Function with the colors green, red and white*/
void b_g_w(void); /**Function with the colors blue, green and white*/

void green(void); 	/**Function that turn on the green LED, waiting and turning it off*/
void blue(void); 	/**Function that turn on the blue LED, waiting and turning it off*/
void purple(void);	/**Function that turn on the purple LED, waiting and turning it off*/
void red(void);		/**Function that turn on the red LED, waiting and turning it off*/
void yellow(void);	/**Function that turn on the yellow LED, waiting and turning it off*/
void white(void);   /**Function that turn on the white LED, waiting and turning it off*/
void no_color(void); /**Functions that doesn't turn on anything**/

#endif /* RGB_H_ */
