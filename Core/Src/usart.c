/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1
#if ( __ARMCC_VERSION >= 6010050 )           /* 使用AC6编译器时 */
__asm( ".global __use_no_semihosting\n\t" ); /* 声明不使用半主机模式 */
__asm( ".global __ARM_use_no_argv \n\t" );   /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import( __use_no_semihosting )

struct __FILE {
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch( int ch ) {
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit( int x ) {
    x = x;
}

char* _sys_command_string( char* cmd, int len ) {
    return NULL;
}

/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* 重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc( int ch, FILE* f ) {
    while ( ( USART1->ISR & 0X40 ) == 0 )
        ; /* 等待上一个字符发送完成 */

    USART1->TDR = ( uint8_t )ch; /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}

#endif

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef  hdma_usart1_rx;
DMA_HandleTypeDef  hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init( void ) {

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance                    = USART1;
    huart1.Init.BaudRate               = 115200;
    huart1.Init.WordLength             = UART_WORDLENGTH_8B;
    huart1.Init.StopBits               = UART_STOPBITS_1;
    huart1.Init.Parity                 = UART_PARITY_NONE;
    huart1.Init.Mode                   = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if ( HAL_UART_Init( &huart1 ) != HAL_OK ) {
        Error_Handler();
    }
    if ( HAL_UARTEx_SetTxFifoThreshold( &huart1, UART_TXFIFO_THRESHOLD_1_8 ) != HAL_OK ) {
        Error_Handler();
    }
    if ( HAL_UARTEx_SetRxFifoThreshold( &huart1, UART_RXFIFO_THRESHOLD_1_8 ) != HAL_OK ) {
        Error_Handler();
    }
    if ( HAL_UARTEx_DisableFifoMode( &huart1 ) != HAL_OK ) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    __HAL_UART_ENABLE_IT( &huart1, UART_IT_IDLE );            // 开启空闲中断
    HAL_UART_Receive_IT( &huart1, ( uint8_t* )&Rx_Temp, 1 );  // 接收中断

    /* USER CODE END USART1_Init 2 */
}

void HAL_UART_MspInit( UART_HandleTypeDef* uartHandle ) {

    GPIO_InitTypeDef         GPIO_InitStruct     = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if ( uartHandle->Instance == USART1 ) {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_USART1;
        PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
        if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK ) {
            Error_Handler();
        }

        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

        /* USART1 DMA Init */
        /* USART1_RX Init */
        hdma_usart1_rx.Instance                 = DMA1_Stream0;
        hdma_usart1_rx.Init.Request             = DMA_REQUEST_USART1_RX;
        hdma_usart1_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_usart1_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_usart1_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_usart1_rx.Init.Mode                = DMA_NORMAL;
        hdma_usart1_rx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_usart1_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if ( HAL_DMA_Init( &hdma_usart1_rx ) != HAL_OK ) {
            Error_Handler();
        }

        __HAL_LINKDMA( uartHandle, hdmarx, hdma_usart1_rx );

        /* USART1_TX Init */
        hdma_usart1_tx.Instance                 = DMA1_Stream1;
        hdma_usart1_tx.Init.Request             = DMA_REQUEST_USART1_TX;
        hdma_usart1_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_usart1_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_usart1_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_usart1_tx.Init.Mode                = DMA_NORMAL;
        hdma_usart1_tx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_usart1_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if ( HAL_DMA_Init( &hdma_usart1_tx ) != HAL_OK ) {
            Error_Handler();
        }

        __HAL_LINKDMA( uartHandle, hdmatx, hdma_usart1_tx );

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority( USART1_IRQn, 0, 0 );
        HAL_NVIC_EnableIRQ( USART1_IRQn );
        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }
}

void HAL_UART_MspDeInit( UART_HandleTypeDef* uartHandle ) {

    if ( uartHandle->Instance == USART1 ) {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit( GPIOA, GPIO_PIN_9 | GPIO_PIN_10 );

        /* USART1 DMA DeInit */
        HAL_DMA_DeInit( uartHandle->hdmarx );
        HAL_DMA_DeInit( uartHandle->hdmatx );

        /* USART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ( USART1_IRQn );
        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
