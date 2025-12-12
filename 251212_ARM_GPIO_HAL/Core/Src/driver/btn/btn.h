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
}Button_Handler;

button_state_t Button_GetState(Button_Handler *Hbtn);

#endif /* SRC_DRIVER_BTN_BTN_H_ */
