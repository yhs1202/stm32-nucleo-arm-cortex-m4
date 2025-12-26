/*
 * common.c
 *
 *  Created on: Dec 18, 2025
 *      Author: kccistc
 */


#include "common.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2){ 	// 1ms call
	  TimeWatch_ISR();
	  StopWatch_ISR();
  }
  else if (htim->Instance == TIM3){ // 0.1s call

  }
}
