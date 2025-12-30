/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "../driver/lcd/lcd.h"
#include "i2c.h"
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
que_t que_task2_to_task3;
que_t que_task3_to_task2; // Circular Queue

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osMutexId lcdMutexHandle;
osSemaphoreId lcdBinarySemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	Que_Init(&que_task2_to_task3);
	Que_Init(&que_task3_to_task2);
	LCD_Init(&hi2c1);
  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of lcdMutex */
  osMutexDef(lcdMutex);
  lcdMutexHandle = osMutexCreate(osMutex(lcdMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of lcdBinarySem */
  osSemaphoreDef(lcdBinarySem);
  lcdBinarySemHandle = osSemaphoreCreate(osSemaphore(lcdBinarySem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityNormal, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityNormal, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
	char msg[50];
	int count = 0;
	int recv;
  /* Infinite loop */
  for(;;)
  {
	  // Send Data Check
	  Que_Enque(&que_task2_to_task3, count);
	  sprintf(msg, "Task02:%d", count++);
//	  osMutexWait(lcdMutexHandle, osWaitForever);
	  osSemaphoreWait(lcdBinarySemHandle, osWaitForever);
	  LCD_WriteStringXY(0, 0, msg);
	  osSemaphoreRelease(lcdBinarySemHandle);
//	  osMutexRelease(lcdMutexHandle);

	  // Recv Data Check
	  if (!Que_isEmpty(&que_task3_to_task2)) {
		  recv = Que_Deque(&que_task3_to_task2);

		  sprintf(msg, "recv:%d", recv);
		  osSemaphoreWait(lcdBinarySemHandle, osWaitForever);
		  LCD_WriteStringXY(0, 9, msg);
		  osSemaphoreRelease(lcdBinarySemHandle);
	  }
    osDelay(1000);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
	char msg[50];
	int count = 0;
	int recv;
  /* Infinite loop */
  for(;;)
  {
	  Que_Enque(&que_task3_to_task2, count);

	  sprintf(msg, "Task03:%d", count++);
//	  osMutexWait(lcdMutexHandle, osWaitForever);
	  osSemaphoreWait(lcdBinarySemHandle, osWaitForever);
	  LCD_WriteStringXY(1, 0, msg);
//	  osMutexRelease(lcdMutexHandle);
	  osSemaphoreRelease(lcdBinarySemHandle);

	  if (!Que_isEmpty(&que_task2_to_task3)) {
		  recv = Que_Deque(&que_task2_to_task3);

		  sprintf(msg, "recv:%d", recv);
		  osSemaphoreWait(lcdBinarySemHandle, osWaitForever);
		  LCD_WriteStringXY(1, 9, msg);
		  osSemaphoreRelease(lcdBinarySemHandle);
	  }
    osDelay(1500);
  }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
