/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32h7xx_it.c
 * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  uint32_t isrflags = READ_REG(huart1.Instance->ISR);
  uint32_t cr1its = READ_REG(huart1.Instance->CR1);
  uint32_t cr3its = READ_REG(huart1.Instance->CR3);

  /* 处理接收中断  */
  if ((isrflags & USART_ISR_RXNE_RXFNE) != RESET)
  {
    /* 从串口接收数据寄存器读取数据存放到接收FIFO */
    uint8_t ch;
    ch = READ_REG(huart1.Instance->RDR);
    uint8_t verify_Flag = 0;

    if (usart1_rx_datalength < buffMaxsize) // 缓冲区内的数据小于缓冲区大小，则写入1字节数据
    {
      circleBuff[w_Ptr] = ch;
      w_Ptr = (w_Ptr + 1) % buffMaxsize; // 循环移动写指针
      usart1_rx_datalength++;            // 每写入1字节数据，缓冲区长度加1
      byteCount++;                       // 字节获取数
    }
    while (usart1_rx_datalength >= 16) // 缓冲区内数据大于帧字节数，则进行数据提取
    {
      verify_Flag = 0;
      // 数据验证
      if ((circleBuff[(r_Ptr + 00) % buffMaxsize] == 0xeb) && // 帧头1
          (circleBuff[(r_Ptr + 01) % buffMaxsize] == 0x90) && // 帧头2
          (circleBuff[(r_Ptr + 14) % buffMaxsize] == 0x0d) && // 帧尾1
          (circleBuff[(r_Ptr + 15) % buffMaxsize] == 0x0a))   // 帧尾2
      {

        // 计算帧数据的校验位
        for (uint8_t i = 0; i < 16; i++)
        {
          frameBuff[i] = circleBuff[(r_Ptr + i) % buffMaxsize]; // 提取潜在帧
          if ((i >= 2) && (i <= 12))
          {
            verify_Flag = verify_Flag + frameBuff[i];
          }
        }

        if (verify_Flag == frameBuff[13]) // 判断校验位
        {
          // 获取正确帧数据
          for (uint8_t i = 0; i < 16; i++)
          {
            USART1_DataBuf[i] = frameBuff[i];
            r_Ptr = (r_Ptr + 1) % buffMaxsize; // 循环移动读指针
            usart1_rx_datalength--;            // 每读取1字节数据，缓冲区数据长度减1
          }
          framecount = (framecount + 1) % 256; // 收到帧数据循环计数
          framecount_get++;                    // 帧计数
        }
        else
        {
          verify_err_cnt++; // 校验不通过
          for (uint8_t i = 0; i < 16; i++)
          {
            error_frame[i] = circleBuff[(r_Ptr + i) % buffMaxsize]; // 保存 校验不通过帧数据
          }
          err2++;
          r_Ptr = (r_Ptr + 1) % buffMaxsize; // 帧验证窗口滑移1字节
          usart1_rx_datalength--;
        }
      }
      else
      {
        for (uint8_t i = 0; i < 16; i++)
        {
          error_frame[i] = circleBuff[(r_Ptr + i) % buffMaxsize];
        }
        err1++; // 未检测到帧头帧尾
        r_Ptr = (r_Ptr + 1) % buffMaxsize;
        usart1_rx_datalength--;
      }
    }
  }

  /* 清除中断标志 */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_PEF);    /*!< Parity Error Clear Flag           */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_FEF);    /*!< Framing Error Clear Flag          */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_NEF);    /*!< Noise Error detected Clear Flag   */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_OREF);   /*!< Overrun Error Clear Flag          */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_IDLEF);  /*!< IDLE line detected Clear Flag     */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_TCF);    /*!< Transmission Complete Clear Flag  */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_TXFECF); /*!< TXFIFO empty clear flag           */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_LBDF);   /*!< LIN Break Detection Clear Flag    */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_CTSF);   /*!< CTS Interrupt Clear Flag          */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_CMF);    /*!< Character Match Clear Flag        */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_WUF);    /*!< Wake Up from stop mode Clear Flag */
  SET_BIT(huart1.Instance->ICR, UART_CLEAR_RTOF);   /*!< UART receiver timeout clear flag  */
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
