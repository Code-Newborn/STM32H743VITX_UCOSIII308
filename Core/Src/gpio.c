/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
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
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_SET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( DHT11_DQ_GPIO_Port, DHT11_DQ_Pin, GPIO_PIN_SET );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = LED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( LED_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = OLED_RST_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( OLED_RST_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = OLED_DC_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( OLED_DC_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = OLED_CS_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( OLED_CS_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = DHT11_DQ_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( DHT11_DQ_GPIO_Port, &GPIO_InitStruct );
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
