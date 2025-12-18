/*
 * clock.c
 *
 *  Created on: Dec 17, 2025
 *      Author: kccistc
 */

#include <stdio.h>
#include "clock.h"
#include <string.h>


extern I2C_HandleTypeDef hi2c1;	// from main
extern TIM_HandleTypeDef htim3;

hBtn btnRunStop;	// left
hBtn btnClear;		// mid
hBtn btnRight;		// right


typedef struct {
	uint8_t hh, mm, ss;
} TW_t;

typedef struct {
	uint8_t mm, ss, ms;
} SW_t;

static volatile TW_t tw = {12, 0, 0};
static volatile SW_t sw = {0, 0, 0};
static volatile uint8_t sw_running = 0;
static volatile uint8_t set_clock = 0;


// Blink flag
static uint8_t blink500 = 1;
static uint8_t blink50  = 1;
static uint32_t t500 = 0;
static uint32_t t50  = 0;

int state = IDLE;
int counter = 0;

void Clock_Init()
{
	LCD_Init(&hi2c1);
	Button_Init(&btnRunStop, GPIOC, GPIO_PIN_10);
	Button_Init(&btnClear, GPIOC, GPIO_PIN_11);
	Button_Init(&btnRight, GPIOC, GPIO_PIN_12);

	sw_running = 0;
	set_clock = 0;
    HAL_TIM_Base_Start_IT(&htim3);

    t500 = HAL_GetTick();
    t50  = HAL_GetTick();
}

void Clock_Excute()
{
	uint32_t now = HAL_GetTick();
	if (now - t500 >= 500) { t500 += 500; blink500 ^= 1; }
	if (now - t50 >= 50) { t50 += 50; blink50 ^= 1;	}

	switch (state)
	{
	case IDLE:
		sw_running = 0;
		set_clock = 0;
		FND_SetNum(sw.ss * 100 + sw.ms);
		if (Button_GetState(&btnRunStop) == ACT_PUSHED) {
			state = SW_RUN;
		}
		if (Button_GetState(&btnClear) == ACT_PUSHED) {
			sw.mm = 0; sw.ss = 0; sw.ms = 0;
		}
		if (Button_GetState(&btnRight) == ACT_PUSHED) {
			set_clock = 1;
			state = SET_CLOCK_HH;
		}
		break;
	case SW_RUN:
		sw_running = 1;
		FND_SetNum(sw.ss * 100 + sw.ms);
		if (Button_GetState(&btnRunStop) == ACT_PUSHED) {
			sw_running = 0;
			state = IDLE;
		}
		break;
	case SET_CLOCK_HH:
		FND_SetNum(1111);
		if (Button_GetState(&btnRunStop) == ACT_PUSHED) {
			if (tw.hh == 23) tw.hh = 0;
			else tw.hh++;
		}
		if (Button_GetState(&btnClear) == ACT_PUSHED) {
			if (tw.hh == 0) tw.hh = 23;
			else tw.hh--;
		}
		if (Button_GetState(&btnRight) == ACT_PUSHED) {
			state = SET_CLOCK_MM;
		}
		break;
	case SET_CLOCK_MM:
		FND_SetNum(2222);
		if (Button_GetState(&btnRunStop) == ACT_PUSHED) {
			if (tw.mm == 59) tw.mm = 0;
			else tw.mm++;
		}
		if (Button_GetState(&btnClear) == ACT_PUSHED) {
			if (tw.mm == 0) tw.mm = 59;
			else tw.mm--;
		}
		if (Button_GetState(&btnRight) == ACT_PUSHED) {
			state = SET_CLOCK_SS;
		}
		break;
	case SET_CLOCK_SS:
		FND_SetNum(3333);
		if (Button_GetState(&btnRunStop) == ACT_PUSHED) {
			if (tw.ss == 59) tw.ss = 0;
			else tw.ss++;
		}
		if (Button_GetState(&btnClear) == ACT_PUSHED) {
			if (tw.ss == 0) tw.ss = 59;
			else tw.ss--;
		}
		if (Button_GetState(&btnRight) == ACT_PUSHED) {
			state = IDLE;
			set_clock = 0;
		}
		break;
	}


	char line1[17], line2[17];

	// ":" Blinking
	char c1 = blink500 && !set_clock ? ' ' : ':';
	char c2 = blink500 && !set_clock ? ' ' : ':';

	char c3 = blink500 && sw_running ? ' ' : ':';
	char c4 = blink50 && sw_running ? ' ' : ':';

	snprintf(line1, sizeof(line1), "TW,    %02d%c%02d%c%02d",
			tw.hh, c1, tw.mm, c2, tw.ss);

	snprintf(line2, sizeof(line2), "SW,    %02d%c%02d%c%02d",
			sw.mm, c3, sw.ss, c4, sw.ms);

	LCD_WriteStringXY(0, 0, line1);
	LCD_WriteStringXY(1, 0, line2);


}

void timeCount(TIM_HandleTypeDef *htim)
{
	// TW
	if (htim->Instance == TIM3)
	{
		static uint8_t tick = 0;	// 1s
		tick++;
		if (tick >= 10)
		{
			tick = 0;
			if(!set_clock)
			{
				tw.ss++;
				if (tw.ss >= 60) { tw.ss = 0; tw.mm++; }
				if (tw.mm >= 60) { tw.mm = 0; tw.hh++; }
				if (tw.hh >= 24) { tw.hh = 0; }
			}
		}
	}
	if (sw_running) {
		 sw.ms++;
		 if (sw.ms >= 10) {
			 sw.ms = 0;
			 sw.ss++;
			 if (sw.ss >= 60) { sw.ss = 0; sw.mm++; }
			 if (sw.mm >= 100) { sw.mm = 0; }
		 }
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3) {
        timeCount(htim);
    }
}
