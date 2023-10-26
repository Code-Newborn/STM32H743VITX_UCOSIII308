/**
 ******************************************************************************
 * @file    usart.h
 * @brief   This file contains all the function prototypes for
 *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

  /* USER CODE BEGIN Includes */

  /* USER CODE END Includes */

  extern UART_HandleTypeDef huart1;

  /* USER CODE BEGIN Private defines */

#define buffMaxsize 60               // 环形缓冲区容量(数据帧字节的倍数)
#define frameSize 16                 // 帧字节数
  extern uint8_t rData[frameSize];   // 中断接收字节数
  extern uint8_t USART1_DataBuf[16]; // 串口数据
  extern uint8_t error_frame[16];    // 错帧内容
  extern uint8_t frameBuff[16];      // 帧内容缓存

  extern uint8_t circleBuff[buffMaxsize]; // 读写 环形缓冲区
  extern uint64_t usart1_rx_datalength;   // 数据长度
  extern uint64_t r_Ptr;                  // 读指针
  extern uint64_t w_Ptr;                  // 写指针
  extern uint64_t framecount_get;         // 帧计数
  extern uint8_t framecount;

  extern uint64_t verify_err_cnt;
  extern uint64_t err1;
  extern uint64_t err2;
  extern uint64_t byteCount;

  extern float data1;
  extern float data2;

  typedef union _Var2Data
  {
    uint8_t x[4];
    float a;
    int b;
  } Var2Data;

  /* USER CODE END Private defines */

  void MX_USART1_UART_Init(void);

  /* USER CODE BEGIN Prototypes */

  /* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
