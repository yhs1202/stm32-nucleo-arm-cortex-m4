/*
 * GPIO.h
 *
 *  Created on: Dec 11, 2025
 *      Author: Hoseung Yoon
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

typedef struct {
	volatile uint32_t MODER;	// mode reg
	volatile uint32_t OTYPER; 	// output type reg
	volatile uint32_t OSPEEDR; 	// output speed reg
	volatile uint32_t PUPDR; 	// pull-up pull-down reg
	volatile uint32_t IDR;		// input data reg
	volatile uint32_t ODR;		// output data reg
	volatile uint32_t BSRR;		// bit set/reset reg
	volatile uint32_t LCKR;		// port configuration lock reg

} GPIO_TypeDef;

typedef enum {OUTPUT, INPUT} pin_mode_t;
typedef enum {GPIO_RESET, GPIO_SET} pin_state_t;

#define GPIOA_OFFSET_ADDR    0x0000
#define GPIOB_OFFSET_ADDR    0x0400
#define GPIOC_OFFSET_ADDR    0x0800
#define GPIOD_OFFSET_ADDR    0x0C00
#define GPIOE_OFFSET_ADDR    0x1000
#define GPIOA_BASE_ADDR      (AHB1_BASE_ADDR + GPIOA_OFFSET_ADDR)
#define GPIOB_BASE_ADDR      (AHB1_BASE_ADDR + GPIOB_OFFSET_ADDR)
#define GPIOC_BASE_ADDR      (AHB1_BASE_ADDR + GPIOC_OFFSET_ADDR)
#define GPIOD_BASE_ADDR      (AHB1_BASE_ADDR + GPIOD_OFFSET_ADDR)
#define GPIOE_BASE_ADDR      (AHB1_BASE_ADDR + GPIOE_OFFSET_ADDR)
#define GPIOA            ((GPIO_TypeDef *)GPIOA_BASE_ADDR)
#define GPIOB            ((GPIO_TypeDef *)GPIOB_BASE_ADDR)
#define GPIOC            ((GPIO_TypeDef *)GPIOC_BASE_ADDR)
#define GPIOD            ((GPIO_TypeDef *)GPIOD_BASE_ADDR)
#define GPIOE            ((GPIO_TypeDef *)GPIOE_BASE_ADDR)

void GPIO_Init(GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t mode);
void GPIO_Write(GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t pinState);
void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint32_t pin);
uint32_t GPIO_Read(GPIO_TypeDef *GPIOx, uint32_t pin);

#endif /* GPIO_H_ */
