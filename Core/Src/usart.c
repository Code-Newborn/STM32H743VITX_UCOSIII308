/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1
#if (__ARMCC_VERSION >= 6010050)           /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t"); /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");   /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
  ch = ch;
  return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
  x = x;
}

char *_sys_command_string(char *cmd, int len)
{
  return NULL;
}

/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* 重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
  while ((USART1->ISR & 0X40) == 0)
    ; /* 等待上一个字符发送完成 */

  USART1->TDR = (uint8_t)ch; /* 将要发送的字符 ch 写入到DR寄存器 */
  return ch;
}

#endif
/***********************************************END*******************************************/

uint8_t rData[frameSize] = {0};     // 中断接收字节
uint8_t USART1_DataFrame[16] = {0}; // 正确帧
uint8_t error_frame[16] = {0};      // 错误帧
uint8_t frameBuff[16] = {0};        // 潜在帧

uint8_t circleBuff[buffMaxsize] = {0}; // 读写 环形缓冲区
uint64_t usart1_rx_datalength = 0;     // 数据长度
uint64_t r_Ptr = 0;                    // 读指针
uint64_t w_Ptr = 0;                    // 写指针
uint64_t framecount_get = 0;           // 帧计数
uint8_t framecount = 0;

uint64_t verify_err_cnt = 0;
uint64_t err1 = 0;
uint64_t err2 = 0;
uint64_t byteCount = 0;

VAR2DATATYPE var2data;

float data1;
float data2;

/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&rData, frameSize); // 开启接收中断
  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1) // 串口1
  {
    uint8_t verify_Flag = 0; // 初始化校验位

    if (usart1_rx_datalength < buffMaxsize) // 缓冲区内的数据小于缓冲区大小，则写入1字节数据
    {
      for (size_t i = 0; i < frameSize; i++)
      {
        circleBuff[w_Ptr] = rData[i];
        w_Ptr = (w_Ptr + 1) % buffMaxsize; // 根据单次中断接收帧字节数，循环移动写指针
        usart1_rx_datalength++;            // 每写入1字节数据，缓冲区长度加1
        byteCount++;                       // 字节获取数
      }
    }
    while (usart1_rx_datalength >= 16) // 缓冲区内数据大于帧字节数，则进行数据提取
    {
      verify_Flag = 0; // 初始化校验位

      if ((circleBuff[(r_Ptr + 00) % buffMaxsize] == 0xeb) && // 帧头1
          (circleBuff[(r_Ptr + 01) % buffMaxsize] == 0x90) && // 帧头2
          (circleBuff[(r_Ptr + 14) % buffMaxsize] == 0x0d) && // 帧尾1
          (circleBuff[(r_Ptr + 15) % buffMaxsize] == 0x0a))   // 帧尾2
      {

        // 计算帧数据的校验位
        for (uint8_t i = 0; i < 16; i++)
        {
          frameBuff[i] = circleBuff[(r_Ptr + i) % buffMaxsize]; // 提取潜在帧 frameBuff
          if ((i >= 2) && (i <= 12))
          {
            verify_Flag = verify_Flag + frameBuff[i]; // 计算帧校验位
          }
        }

        if (verify_Flag == frameBuff[13]) // 校验位正确，获取正确帧数据
        {
          for (uint8_t i = 0; i < 16; i++)
          {
            USART1_DataFrame[i] = frameBuff[i]; // 获取正确帧 USART1_DataFrame
            r_Ptr = (r_Ptr + 1) % buffMaxsize;  // 循环移动读指针
            usart1_rx_datalength--;             // 每读取1字节数据，缓冲区数据长度减1
          }

          // 提取正确帧 数据1
          var2data.UCharData[3] = USART1_DataFrame[4];
          var2data.UCharData[2] = USART1_DataFrame[5];
          var2data.UCharData[1] = USART1_DataFrame[6];
          var2data.UCharData[0] = USART1_DataFrame[7];
          data1 = var2data.FloatVal;

          // 提取正确帧 数据2
          var2data.UCharData[3] = USART1_DataFrame[8];
          var2data.UCharData[2] = USART1_DataFrame[9];
          var2data.UCharData[1] = USART1_DataFrame[10];
          var2data.UCharData[0] = USART1_DataFrame[11];
          data2 = var2data.FloatVal;

          framecount = (framecount + 1) % 256; // 收到帧数据循环计数
          framecount_get++;                    // 收到帧计数
        }
        else
        {
          verify_err_cnt++; // 校验不通过
          for (uint8_t i = 0; i < 16; i++)
          {
            error_frame[i] = circleBuff[(r_Ptr + i) % buffMaxsize]; // 保存 校验不通过时的 数据内容
          }
          err2++;
          r_Ptr = (r_Ptr + 1) % buffMaxsize; // 帧验证窗口滑移1字节
          usart1_rx_datalength--;
        }
      }
      else // 未检测到帧头帧尾
      {
        for (uint8_t i = 0; i < 16; i++)
        {
          error_frame[i] = circleBuff[(r_Ptr + i) % buffMaxsize]; // 保存 未检测到帧头帧尾的 数据内容
        }
        err1++;
        r_Ptr = (r_Ptr + 1) % buffMaxsize;
        usart1_rx_datalength--;
      }
    }
    memset(rData, '\0', sizeof(rData));                         // 清除接收数据
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&rData, frameSize); // 重新开启接收中断
  }
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
