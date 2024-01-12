/**
 ******************************************************************************
 * @file    dcmi.c
 * @brief   This file provides code for the configuration
 *          of the DCMI instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
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
#include "dcmi.h"

/* USER CODE BEGIN 0 */
#include "lcd.h"
/* USER CODE END 0 */

DCMI_HandleTypeDef hdcmi;
DMA_HandleTypeDef  hdma_dcmi;

/* DCMI init function */
void MX_DCMI_Init( void ) {

    /* USER CODE BEGIN DCMI_Init 0 */

    /* USER CODE END DCMI_Init 0 */

    /* USER CODE BEGIN DCMI_Init 1 */

    /* USER CODE END DCMI_Init 1 */
    hdcmi.Instance              = DCMI;
    hdcmi.Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
    hdcmi.Init.PCKPolarity      = DCMI_PCKPOLARITY_FALLING;
    hdcmi.Init.VSPolarity       = DCMI_VSPOLARITY_LOW;
    hdcmi.Init.HSPolarity       = DCMI_HSPOLARITY_LOW;
    hdcmi.Init.CaptureRate      = DCMI_CR_ALL_FRAME;
    hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
    hdcmi.Init.JPEGMode         = DCMI_JPEG_DISABLE;
    hdcmi.Init.ByteSelectMode   = DCMI_BSM_ALL;
    hdcmi.Init.ByteSelectStart  = DCMI_OEBS_ODD;
    hdcmi.Init.LineSelectMode   = DCMI_LSM_ALL;
    hdcmi.Init.LineSelectStart  = DCMI_OELS_ODD;
    if ( HAL_DCMI_Init( &hdcmi ) != HAL_OK ) {
        Error_Handler();
    }
    /* USER CODE BEGIN DCMI_Init 2 */

    __HAL_DCMI_ENABLE_IT( &hdcmi, DCMI_IT_FRAME ); /* 开启镜头捕捉中断 */
    __HAL_DCMI_ENABLE( &hdcmi );

    /* 关闭行中断、VSYNC中断、同步错误中断和溢出中断 */
    __HAL_DCMI_DISABLE_IT( &hdcmi, DCMI_IT_LINE | DCMI_IT_VSYNC | DCMI_IT_ERR | DCMI_IT_OVR );

    /* USER CODE END DCMI_Init 2 */
}

void HAL_DCMI_MspInit( DCMI_HandleTypeDef* dcmiHandle ) {

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if ( dcmiHandle->Instance == DCMI ) {
        /* USER CODE BEGIN DCMI_MspInit 0 */

        /* USER CODE END DCMI_MspInit 0 */
        /* DCMI clock enable */
        __HAL_RCC_DCMI_CLK_ENABLE();

        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**DCMI GPIO Configuration
        PE4     ------> DCMI_D4
        PE5     ------> DCMI_D6
        PE6     ------> DCMI_D7
        PA4     ------> DCMI_HSYNC
        PA6     ------> DCMI_PIXCLK
        PC6     ------> DCMI_D0
        PC7     ------> DCMI_D1
        PC9     ------> DCMI_D3
        PD3     ------> DCMI_D5
        PB7     ------> DCMI_VSYNC
        PE0     ------> DCMI_D2
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_0;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init( GPIOE, &GPIO_InitStruct );

        GPIO_InitStruct.Pin       = GPIO_PIN_4 | GPIO_PIN_6;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

        GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

        GPIO_InitStruct.Pin       = GPIO_PIN_3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init( GPIOD, &GPIO_InitStruct );

        GPIO_InitStruct.Pin       = GPIO_PIN_7;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );

        /* DCMI DMA Init */
        /* DCMI Init */
        hdma_dcmi.Instance                 = DMA1_Stream1;
        hdma_dcmi.Init.Request             = DMA_REQUEST_DCMI;
        hdma_dcmi.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_dcmi.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_dcmi.Init.MemInc              = DMA_MINC_DISABLE;
        hdma_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_dcmi.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_dcmi.Init.Mode                = DMA_CIRCULAR;
        hdma_dcmi.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_dcmi.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
        hdma_dcmi.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
        hdma_dcmi.Init.MemBurst            = DMA_MBURST_SINGLE;
        hdma_dcmi.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        if ( HAL_DMA_Init( &hdma_dcmi ) != HAL_OK ) {
            Error_Handler();
        }

        __HAL_LINKDMA( dcmiHandle, DMA_Handle, hdma_dcmi );

        /* DCMI interrupt Init */
        HAL_NVIC_SetPriority( DCMI_IRQn, 2, 0 );
        HAL_NVIC_EnableIRQ( DCMI_IRQn );
        /* USER CODE BEGIN DCMI_MspInit 1 */

        /* 在开启DMA之前先使用__HAL_UNLOCK()解锁一次DMA,因为HAL_DMA_Statrt()HAL_DMAEx_MultiBufferStart()
         * 这两个函数一开始要先使用__HAL_LOCK()锁定DMA,而函数__HAL_LOCK()会判断当前的DMA状态是否为锁定状态，如果是
         * 锁定状态的话就直接返回HAL_BUSY，这样会导致函数HAL_DMA_Statrt()和HAL_DMAEx_MultiBufferStart()后续的DMA配置
         * 程序直接被跳过！DMA也就不能正常工作，为了避免这种现象，所以在启动DMA之前先调用__HAL_UNLOCK()先解锁一次DMA。
         */
        __HAL_UNLOCK( &hdma_dcmi );

        if ( mem1addr == 0 ) /* 开启DMA，不使用双缓冲 */
        {
            HAL_DMA_Start( &hdma_dcmi, ( uint32_t )&DCMI->DR, ( uint32_t )&LCD->LCD_RAM, 1 );
        } else /* 使用双缓冲 */
        {
            HAL_DMAEx_MultiBufferStart( &hdma_dcmi, ( uint32_t )&DCMI->DR, ( uint32_t )&LCD->LCD_RAM, 0, 1 ); /* 开启双缓冲 */
            __HAL_DMA_ENABLE_IT( &hdma_dcmi, DMA_IT_TC );                                                     /* 开启传输完成中断 */
            HAL_NVIC_SetPriority( DMA1_Stream1_IRQn, 2, 3 );                                                  /* DMA中断优先级 */
            HAL_NVIC_EnableIRQ( DMA1_Stream1_IRQn );
        }

        /* USER CODE END DCMI_MspInit 1 */
    }
}

void HAL_DCMI_MspDeInit( DCMI_HandleTypeDef* dcmiHandle ) {

    if ( dcmiHandle->Instance == DCMI ) {
        /* USER CODE BEGIN DCMI_MspDeInit 0 */

        /* USER CODE END DCMI_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_DCMI_CLK_DISABLE();

        /**DCMI GPIO Configuration
        PE4     ------> DCMI_D4
        PE5     ------> DCMI_D6
        PE6     ------> DCMI_D7
        PA4     ------> DCMI_HSYNC
        PA6     ------> DCMI_PIXCLK
        PC6     ------> DCMI_D0
        PC7     ------> DCMI_D1
        PC9     ------> DCMI_D3
        PD3     ------> DCMI_D5
        PB7     ------> DCMI_VSYNC
        PE0     ------> DCMI_D2
        */
        HAL_GPIO_DeInit( GPIOE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_0 );

        HAL_GPIO_DeInit( GPIOA, GPIO_PIN_4 | GPIO_PIN_6 );

        HAL_GPIO_DeInit( GPIOC, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 );

        HAL_GPIO_DeInit( GPIOD, GPIO_PIN_3 );

        HAL_GPIO_DeInit( GPIOB, GPIO_PIN_7 );

        /* DCMI DMA DeInit */
        HAL_DMA_DeInit( dcmiHandle->DMA_Handle );

        /* DCMI interrupt Deinit */
        HAL_NVIC_DisableIRQ( DCMI_IRQn );
        /* USER CODE BEGIN DCMI_MspDeInit 1 */

        /* USER CODE END DCMI_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/**
 * @brief       DCMI,启动传输
 * @param       无
 * @retval      无
 */
void dcmi_start( void ) {
    if ( lcddev.wramcmd != 0 ) {
        lcd_set_cursor( 0, 0 );  /* 设置坐标到原点 */
        lcd_write_ram_prepare(); /* 开始写入GRAM */
    }

    __HAL_DMA_ENABLE( &hdma_dcmi ); /* 使能DMA */
    DCMI->CR |= DCMI_CR_CAPTURE;    /* DCMI捕获使能 */
}

/**
 * @brief       DCMI,关闭传输
 * @param       无
 * @retval      无
 */
void dcmi_stop( void ) {
    DCMI->CR &= ~( DCMI_CR_CAPTURE ); /* DCMI捕获关闭 */

    while ( DCMI->CR & 0X01 )
        ; /* 等待传输结束 */

    __HAL_DMA_DISABLE( &hdma_dcmi ); /* 关闭DMA */
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
