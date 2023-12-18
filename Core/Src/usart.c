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

/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */
#include <stdio.h>

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
FILE __stdout, __stdin;

/* 重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
// int fputc( int ch, FILE* f ) {
//     while ( ( USART1->ISR & 0X40 ) == 0 )  // INFO 使用USART1
//         ;                                  /* 等待上一个字符发送完成 */

//     USART1->TDR = ( uint8_t )ch; /* 将要发送的字符 ch 写入到DR寄存器 */
//     return ch;
// }

/* 重定向c库函数printf到串口USARTx，重定向后可使用printf函数 */
int fputc( int ch, FILE* f ) {
    /* 发送一个字节数据到串口USARTx */
    HAL_UART_Transmit( &huart1, ( uint8_t* )&ch, 1, 0xFFFF );  // INFO 使用USART1
    return ( ch );
}

/* 重定向c库函数scanf到串口USARTx，重写向后可使用scanf、getchar等函数 */
int fgetc( FILE* f ) {
    int ch;
    /* 等待串口输入数据 */
    while ( __HAL_UART_GET_FLAG( &huart1, UART_FLAG_RXNE ) == RESET )
        ;
    __HAL_UART_CLEAR_OREFLAG( &huart1 );
    HAL_UART_Receive( &huart1, ( uint8_t* )&ch, 1, 0xFFFF );
    return ( ch );
}

#endif
/***********************************************END*******************************************/

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

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

    /* USER CODE END USART1_Init 2 */
}
/* USART2 init function */

void MX_USART2_UART_Init( void ) {

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance                    = USART2;
    huart2.Init.BaudRate               = 115200;
    huart2.Init.WordLength             = UART_WORDLENGTH_8B;
    huart2.Init.StopBits               = UART_STOPBITS_1;
    huart2.Init.Parity                 = UART_PARITY_NONE;
    huart2.Init.Mode                   = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if ( HAL_UART_Init( &huart2 ) != HAL_OK ) {
        Error_Handler();
    }
    if ( HAL_UARTEx_SetTxFifoThreshold( &huart2, UART_TXFIFO_THRESHOLD_1_8 ) != HAL_OK ) {
        Error_Handler();
    }
    if ( HAL_UARTEx_SetRxFifoThreshold( &huart2, UART_RXFIFO_THRESHOLD_1_8 ) != HAL_OK ) {
        Error_Handler();
    }
    if ( HAL_UARTEx_DisableFifoMode( &huart2 ) != HAL_OK ) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */
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

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority( USART1_IRQn, 1, 0 );
        HAL_NVIC_EnableIRQ( USART1_IRQn );
        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    } else if ( uartHandle->Instance == USART2 ) {
        /* USER CODE BEGIN USART2_MspInit 0 */

        /* USER CODE END USART2_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_USART2;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
        if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK ) {
            Error_Handler();
        }

        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority( USART2_IRQn, 2, 0 );
        HAL_NVIC_EnableIRQ( USART2_IRQn );
        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
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

        /* USART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ( USART1_IRQn );
        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    } else if ( uartHandle->Instance == USART2 ) {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        HAL_GPIO_DeInit( GPIOA, GPIO_PIN_2 | GPIO_PIN_3 );

        /* USART2 interrupt Deinit */
        HAL_NVIC_DisableIRQ( USART2_IRQn );
        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
