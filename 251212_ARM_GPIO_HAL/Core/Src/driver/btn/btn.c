/*
 * btn.c
 *
 *  Created on: Dec 12, 2025
 *      Author: Hoseung Yoon
 */


#include "btn.h"
enum {PUSHED, RELEASED};

//button_state_t Button_GetState(GPIO_TypeDef* GPIOx, uint32_t pinNum)
button_state_t Button_GetState(Button_Handler* Hbtn)
{
	static uint32_t prevState = RELEASED;
	uint32_t curState;
	curState = HAL_GPIO_ReadPin(Hbtn->GPIOx, Hbtn->GPIO_Pin);

	if ((prevState == RELEASED) && (curState == PUSHED)) {
		HAL_Delay(2); // debounce
		prevState = PUSHED;
		return ACT_PUSHED;
	}
	else if ((prevState == PUSHED) && (curState == RELEASED)) {
		HAL_Delay(2); // debounce
		prevState = RELEASED;
		return ACT_RELEASED;
	}
	return NO_ACT;
}
