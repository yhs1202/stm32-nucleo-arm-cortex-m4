/*
 * clock.h
 *
 *  Created on: Dec 17, 2025
 *      Author: kccistc
 */

#ifndef SRC_AP_CLOCK_H_
#define SRC_AP_CLOCK_H_

#include "stm32f4xx_hal.h"
#include "../driver/btn/btn.h"
#include "../driver/fnd/fnd.h"
#include "../driver/lcd/lcd.h"

enum {IDLE, SW_RUN, SET_CLOCK_HH, SET_CLOCK_MM, SET_CLOCK_SS};

void Clock_Init();
void Clock_Excute();
void timeCount(TIM_HandleTypeDef *htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* SRC_AP_CLOCK_H_ */
