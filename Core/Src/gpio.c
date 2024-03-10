/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/** 串口 USART1
     PA9  ------>  USART1_TX
     PA10 ------>  USART1_RX
*/

/** 0.96寸 OLED 显示屏 硬件SPI1
     PA7  ------>  SDA  ------>  SPI1_MOSI  数据 主发从收
     PA5  ------>  SCK  ------>  SPI1_SCK   时钟 主发从收
     PB0  ------>  CS   ------>  OLED_CS    从机 片选
     PC5  ------>  DC   ------>  OLED_DC    从机 数据/命令标志
     PC4  ------>  Reset------>  OLED_RST   从机 复位
     3.3V ------>  VDD  ------>
     GND  ------>  GND  ------>
*/

/** 温湿度传感器DHT11引脚配置
     PB1  ------> DHT_DQ  ------> DHT_DOUT
*/

/* USER CODE END 1 */

/** Configure pins
     PC14-OSC32_IN (OSC32_IN)   ------> RCC_OSC32_IN
     PC15-OSC32_OUT (OSC32_OUT)   ------> RCC_OSC32_OUT
     PH0-OSC_IN (PH0)   ------> RCC_OSC_IN
     PH1-OSC_OUT (PH1)   ------> RCC_OSC_OUT
     PA13 (JTMS/SWDIO)   ------> DEBUG_JTMS-SWDIO
     PA14 (JTCK/SWCLK)   ------> DEBUG_JTCK-SWCLK
*/
void MX_GPIO_Init( void ) {

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_SET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( DHT11_DQ_GPIO_Port, DHT11_DQ_Pin, GPIO_PIN_SET );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = LCD_BLK_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( LCD_BLK_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pins : PCPin PCPin */
    GPIO_InitStruct.Pin  = KEY_MID_Pin | KEY_SET_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = LED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( LED_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = LCD_RST_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( LCD_RST_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = LCD_DC_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( LCD_DC_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = LCD_CS_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( LCD_CS_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = DHT11_DQ_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( DHT11_DQ_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin  = KEY_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init( KEY_RST_GPIO_Port, &GPIO_InitStruct );
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
