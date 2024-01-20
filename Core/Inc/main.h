/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
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
#include "string.h"
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
void Error_Handler( void );

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_MID_Pin        GPIO_PIN_13
#define KEY_MID_GPIO_Port  GPIOC
#define LED_Pin            GPIO_PIN_1
#define LED_GPIO_Port      GPIOA
#define OLED_RST_Pin       GPIO_PIN_6
#define OLED_RST_GPIO_Port GPIOA
#define OLED_DC_Pin        GPIO_PIN_4
#define OLED_DC_GPIO_Port  GPIOC
#define OLED_CS_Pin        GPIO_PIN_0
#define OLED_CS_GPIO_Port  GPIOB
#define DHT11_DQ_Pin       GPIO_PIN_11
#define DHT11_DQ_GPIO_Port GPIOD
#define KEY_SET_Pin        GPIO_PIN_12
#define KEY_SET_GPIO_Port  GPIOC
#define KEY_RST_Pin        GPIO_PIN_6
#define KEY_RST_GPIO_Port  GPIOD

/* USER CODE BEGIN Private defines */

#define NumOfTask 2
extern unsigned int Task_Delay[ NumOfTask ];

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
