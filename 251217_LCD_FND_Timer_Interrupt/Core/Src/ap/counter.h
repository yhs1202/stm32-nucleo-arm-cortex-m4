/*
 * counter.h
 *
 *  Created on: Dec 17, 2025
 *      Author: Hoseung Yoon
 */

#ifndef SRC_AP_COUNTER_H_
#define SRC_AP_COUNTER_H_

#include "stm32f4xx_hal.h"
#include "../driver/btn/btn.h"
#include "../driver/fnd/fnd.h"
#include "../driver/lcd/lcd.h"

enum {STOP, RUN, CLEAR};

void Counter_Init();
void Counter_Excute();

#endif /* SRC_AP_COUNTER_H_ */
