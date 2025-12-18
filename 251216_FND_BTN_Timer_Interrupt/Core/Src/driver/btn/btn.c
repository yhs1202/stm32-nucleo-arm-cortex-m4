/*
 * btn.c
 *
 *  Created on: Dec 12, 2025
 *      Author: Hoseung Yoon
 */


#include "btn.h"
enum {PUSHED, RELEASED};

void Button_Init(hBtn *Hbtn, GPIO_TypeDef* GPIOx, uint32_t pinNum)
{
	Hbtn->GPIOx = GPIOx;
	Hbtn->GPIO_Pin = pinNum;
	Hbtn->prevState = RELEASED;
}

button_state_t Button_GetState(hBtn* Hbtn)
{
//	static uint32_t prevState = RELEASED;
	uint32_t curState;
	curState = HAL_GPIO_ReadPin(Hbtn->GPIOx, Hbtn->GPIO_Pin);

	if ((Hbtn->prevState == RELEASED) && (curState == PUSHED)) {
		HAL_Delay(2); // debounce
		Hbtn->prevState = PUSHED;
		return ACT_PUSHED;
	}
	else if ((Hbtn->prevState == PUSHED) && (curState == RELEASED)) {
		HAL_Delay(2); // debounce
		Hbtn->prevState = RELEASED;
		return ACT_RELEASED;
	}
	return NO_ACT;
}
