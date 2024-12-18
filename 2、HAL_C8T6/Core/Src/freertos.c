/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "PCA9685.h"
#include "ssd1306.h"
#include "globals.h"
#include "SR04.h"
#include "mpu6050.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char distance_buffer[20];  // 用来存储格式化后的距离字符串
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for active */
osThreadId_t activeHandle;
const osThreadAttr_t active_attributes = {
  .name = "active",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for activeEvent */
osEventFlagsId_t activeEventHandle;
const osEventFlagsAttr_t activeEvent_attributes = {
  .name = "activeEvent"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void active_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of active */
  activeHandle = osThreadNew(active_task, NULL, &active_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of activeEvent */
  activeEventHandle = osEventFlagsNew(&activeEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;) {
    // GyroData gyroDataStruct;
    display_oled();
    osDelay(10);
    // advance_bot_init();
    // MPU6050_GetAccelData(&gyroDataStruct);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_active_task */
/**
* @brief Function implementing the active thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_active_task */
void active_task(void *argument)
{
  /* USER CODE BEGIN active_task */
  /* Infinite loop */
  for(;;) {
    // 创建结构体变量用于存储陀螺仪数据
    // GyroData gyroDataStruct;
    osEventFlagsWait(activeEventHandle, 0x01, osFlagsWaitAny, osWaitForever);
    while(1) {
      switch(activeTask) {
        case 1:
          ssd1306_clear_area(60,20,128,40);
          ssd1306_set_cursor(60, 35);
          ssd1306_write_string(font6x8, "advance  ");
          if(SR04_GetData() <= 40) {
            HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
            advance_bot_init();
          } else {
            advance();
          }
          break;
        case 2:
          ssd1306_clear_area(60,20,250,40);
          ssd1306_set_cursor(60, 35);
          ssd1306_write_string(font6x8, "turn left");
          turn_left();
          break;
        case 3:
          ssd1306_clear_area(60,20,250,40);
          ssd1306_set_cursor(60, 35);
          ssd1306_write_string(font6x8, "turn right");
          turn_right();
          break;
        case 4:
          ssd1306_clear_area(60,20,250,40);
          ssd1306_set_cursor(60, 35);
          ssd1306_write_string(font6x8, "back      ");
          back();
          break;
        case 5:
          ssd1306_clear_area(60,20,250,40);
          ssd1306_set_cursor(60, 35);
          ssd1306_write_string(font6x8, "stop      ");
          advance_bot_init();
          break;
        default:
          break;
      }
      osDelay(100);
    }
  }
  /* USER CODE END active_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */

