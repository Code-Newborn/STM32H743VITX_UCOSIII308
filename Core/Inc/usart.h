/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */

#define buffMaxsize 60                // 环形缓冲区容量(数据帧字节的倍数)
#define frameSize   16                // 帧字节数
extern uint8_t rData[ frameSize ];    // 中断接收字节数
extern uint8_t USART1_DataBuf[ 16 ];  // 串口数据
extern uint8_t error_frame[ 16 ];     // 错帧内容
extern uint8_t frameBuff[ 16 ];       // 帧内容缓存

extern uint8_t  circleBuff[ buffMaxsize ];  // 读写 环形缓冲区
extern uint64_t usart1_rx_datalength;       // 数据长度
extern uint64_t r_Ptr;                      // 读指针
extern uint64_t w_Ptr;                      // 写指针
extern uint64_t framecount_get;             // 帧计数
extern uint8_t  framecount;

extern uint64_t verify_err_cnt;
extern uint64_t err1;
extern uint64_t err2;
extern uint64_t byteCount;

extern float data1;
extern float data2;

typedef union {
    unsigned char  UCharData[ 8 ];  // Data 8 Bytes
    unsigned short UShortVal;       // 2 Bytes
    unsigned int   UIntVal;         // 4 Bytes
    unsigned long  ULongVal;        // 4 Bytes
    float          FloatVal;        // 4 Bytes
    double         DoubleVal;       // 8 Bytes
} VAR2DATATYPE;

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

