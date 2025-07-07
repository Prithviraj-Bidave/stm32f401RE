/*
 * tim.h
 *
 *  Created on: Nov 9, 2024
 *      Author: Prithviraj
 */

#ifndef TIM_H_
#define TIM_H_

void tim2_1hz_init(void);
void tim2_pa5_output_compare(void);
void tim3_pa6_input_capture(void);
void tim3_pa6_pa7_input_capture(void);
void tim2_pa5_pa1_output_compare(void);



#define SR_UIF		(1U<<0)
#define SR_CC1IF	(1U<<1)
#define SR_CC2IF 	(1U << 2)

#endif /* TIM_H_ */
