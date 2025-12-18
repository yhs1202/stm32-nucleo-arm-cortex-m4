/*
 * btn.h
 *
 *  Created on: Dec 12, 2025
 *      Author: Hoseung Yoon
 */

#ifndef SRC_DRIVER_BTN_BTN_H_
#define SRC_DRIVER_BTN_BTN_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef enum {NO_ACT, ACT_PUSHED, ACT_RELEASED} button_state_t;

typedef struct {
	GPIO_TypeDef* GPIOx;
	uint32_t GPIO_Pin;
	uint32_t prevState;
}hBtn;

void Button_Init(hBtn *Hbtn, GPIO_TypeDef* GPIOx, uint32_t pinNum);
button_state_t Button_GetState(hBtn *Hbtn);

#endif /* SRC_DRIVER_BTN_BTN_H_ */
