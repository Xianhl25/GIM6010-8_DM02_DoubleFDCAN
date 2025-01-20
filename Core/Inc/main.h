/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Power_OUT2_EN_Pin GPIO_PIN_13
#define Power_OUT2_EN_GPIO_Port GPIOC
#define Power_OUT1_EN_Pin GPIO_PIN_14
#define Power_OUT1_EN_GPIO_Port GPIOC
#define Power_5V_EN_Pin GPIO_PIN_15
#define Power_5V_EN_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_15
#define LCD_CS_GPIO_Port GPIOE
#define LCD_BLK_Pin GPIO_PIN_10
#define LCD_BLK_GPIO_Port GPIOB
#define LCD_RES_Pin GPIO_PIN_11
#define LCD_RES_GPIO_Port GPIOB
#define LCD_DC_Pin GPIO_PIN_10
#define LCD_DC_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
#define LEFT_MOTOR_TORQUE_CONSTANT 0.43 /* 电机扭矩系数，用于根据反馈的电流实时计算电机扭矩大小 */
#define RIGHT_MOTOR_TORQUE_CONSTANT 0.43 /* 电机扭矩系数，用于根据反馈的电流实时计算电机扭矩大小 */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
