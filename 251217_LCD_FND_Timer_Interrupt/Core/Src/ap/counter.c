/*
 * counter.c
 *
 *  Created on: Dec 17, 2025
 *      Author: Hoseung Yoon
 */

#include <stdio.h>
#include "counter.h"



extern I2C_HandleTypeDef hi2c1;	// from main
extern TIM_HandleTypeDef htim3;

uint16_t counter = 0;

hBtn btnRunStop;
hBtn btnClear;

int state = STOP;


void Counter_Init()
{
	LCD_Init(&hi2c1);
	Button_Init(&btnRunStop, GPIOC, GPIO_PIN_10);
	Button_Init(&btnClear, GPIOC, GPIO_PIN_11);
}

void Counter_Excute()
{
	char str[80];
	sprintf(str, "%d Hello KCCISTC", 2025);
	LCD_WriteStringXY(0, 0, str);
	FND_SetNum(counter);
	switch(state)
	{
	case STOP:
	  HAL_TIM_Base_Stop_IT(&htim3);
	  sprintf(str, "%04d", counter);
	  LCD_WriteStringXY(1, 0, str);
	  if (Button_GetState(&btnRunStop) == ACT_PUSHED) {
		  state = RUN;
	  }
	  if (Button_GetState(&btnClear) == ACT_PUSHED){
		  state = CLEAR;
	  }
	  break;
	case RUN:
	  HAL_TIM_Base_Start_IT(&htim3);
	  sprintf(str, "%04d", counter);
	  LCD_WriteStringXY(1, 0, str);
	  if (Button_GetState(&btnRunStop) == ACT_PUSHED) {
		  state = STOP;
	  }
	  break;
	case CLEAR:
	  counter = 0;
	  sprintf(str, "%04d", counter);
	  LCD_WriteStringXY(1, 0, str);
	  state = STOP;
	  break;
	}
}
