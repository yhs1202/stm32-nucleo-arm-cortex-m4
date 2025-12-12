/*
 * GPIO.c
 *
 *  Created on: Dec 11, 2025
 *      Author: Hoseung Yoon
 */


#include "GPIO.h"

void GPIO_Init(GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t mode){
   if(mode == OUTPUT){
      GPIOx->MODER |= (0x01 << pin*2);
      GPIOx->MODER &= ~(0x02 << pin*2);
   }
   else{
      GPIOx->MODER &= ~(0x03 << pin*2);
   }
}

void GPIO_Write(GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t pinState){
   if(pinState){
      GPIOx->ODR |= (1<<pin);
   }
   else {
      GPIOx->ODR &= ~(1<<pin);
   }
}

void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint32_t pin){
   GPIOx->ODR ^= (1<<pin);
}

uint32_t GPIO_Read(GPIO_TypeDef *GPIOx, uint32_t pin){
   if((GPIOx->IDR & (1<<pin)) == 0){
      return 0;
   }
   else {
      return 1;
   }
}
