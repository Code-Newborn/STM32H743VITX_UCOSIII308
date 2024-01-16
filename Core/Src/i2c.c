/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    i2c.c
 * @brief   This file provides code for the configuration
 *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c2;

/* I2C2 init function */
void MX_I2C2_Init( void ) {

    /* USER CODE BEGIN I2C2_Init 0 */

    /* USER CODE END I2C2_Init 0 */

    /* USER CODE BEGIN I2C2_Init 1 */

    /* USER CODE END I2C2_Init 1 */
    hi2c2.Instance              = I2C2;
    hi2c2.Init.Timing           = 0x10C0ECFF;
    hi2c2.Init.OwnAddress1      = 0;
    hi2c2.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.OwnAddress2      = 0;
    hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c2.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if ( HAL_I2C_Init( &hi2c2 ) != HAL_OK ) {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if ( HAL_I2CEx_ConfigAnalogFilter( &hi2c2, I2C_ANALOGFILTER_ENABLE ) != HAL_OK ) {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if ( HAL_I2CEx_ConfigDigitalFilter( &hi2c2, 0 ) != HAL_OK ) {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C2_Init 2 */

    /* USER CODE END I2C2_Init 2 */
}

void HAL_I2C_MspInit( I2C_HandleTypeDef* i2cHandle ) {

    GPIO_InitTypeDef         GPIO_InitStruct     = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if ( i2cHandle->Instance == I2C2 ) {
        /* USER CODE BEGIN I2C2_MspInit 0 */

        /* USER CODE END I2C2_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
        PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
        if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK ) {
            Error_Handler();
        }

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C2 GPIO Configuration
        PB10     ------> I2C2_SCL
        PB11     ------> I2C2_SDA
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );

        /* I2C2 clock enable */
        __HAL_RCC_I2C2_CLK_ENABLE();
        /* USER CODE BEGIN I2C2_MspInit 1 */

        /* USER CODE END I2C2_MspInit 1 */
    }
}

void HAL_I2C_MspDeInit( I2C_HandleTypeDef* i2cHandle ) {

    if ( i2cHandle->Instance == I2C2 ) {
        /* USER CODE BEGIN I2C2_MspDeInit 0 */

        /* USER CODE END I2C2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C2_CLK_DISABLE();

        /**I2C2 GPIO Configuration
        PB10     ------> I2C2_SCL
        PB11     ------> I2C2_SDA
        */
        HAL_GPIO_DeInit( GPIOB, GPIO_PIN_10 );

        HAL_GPIO_DeInit( GPIOB, GPIO_PIN_11 );

        /* USER CODE BEGIN I2C2_MspDeInit 1 */

        /* USER CODE END I2C2_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/**
 * @brief  Manages error callback by re-initializing I2C.
 * @param  Addr: I2C Address
 * @retval None
 */
static void I2Cx_Error( uint8_t Addr ) {
    /* 恢复I2C寄存器为默认值 */
    HAL_I2C_DeInit( &hi2c2 );
    /* 重新初始化I2C外设 */
    MX_I2C2_Init();
}
/**
 * @brief  写寄存器，这是提供给上层的接口
 * @param  slave_addr: 从机地址
 * @param 	reg_addr:寄存器地址
 * @param len：写入的长度
 *	@param data_ptr:指向要写入的数据
 * @retval 正常为0，不正常为非0
 */
int Sensors_I2C_WriteRegister( unsigned char slave_addr, unsigned char reg_addr, unsigned short len, unsigned char* data_ptr ) {
    HAL_StatusTypeDef status = HAL_OK;
    status                   = HAL_I2C_Mem_Write( &hi2c2, slave_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data_ptr, len, 1000 );
    /* 检查通讯状态 */
    if ( status != HAL_OK ) {
        /* 总线出错处理 */
        I2Cx_Error( slave_addr );
    }
    while ( HAL_I2C_GetState( &hi2c2 ) != HAL_I2C_STATE_READY ) {
    }
    /* 检查SENSOR是否就绪进行下一次读写操作 */
    while ( HAL_I2C_IsDeviceReady( &hi2c2, slave_addr, 1000, 1000 ) == HAL_TIMEOUT )
        ;
    /* 等待传输结束 */
    while ( HAL_I2C_GetState( &hi2c2 ) != HAL_I2C_STATE_READY ) {
    }
    return status;
}

/**
 * @brief  读寄存器，这是提供给上层的接口
 * @param  slave_addr: 从机地址
 * @param 	reg_addr:寄存器地址
 * @param len：要读取的长度
 *	@param data_ptr:指向要存储数据的指针
 * @retval 正常为0，不正常为非0
 */
int Sensors_I2C_ReadRegister( unsigned char slave_addr, unsigned char reg_addr, unsigned short len, unsigned char* data_ptr ) {
    HAL_StatusTypeDef status = HAL_OK;
    status                   = HAL_I2C_Mem_Read( &hi2c2, slave_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data_ptr, len, 1000 );
    /* 检查通讯状态 */
    if ( status != HAL_OK ) {
        /* 总线出错处理 */
        I2Cx_Error( slave_addr );
    }
    while ( HAL_I2C_GetState( &hi2c2 ) != HAL_I2C_STATE_READY ) {
    }
    /* 检查SENSOR是否就绪进行下一次读写操作 */
    while ( HAL_I2C_IsDeviceReady( &hi2c2, slave_addr, 1000, 1000 ) == HAL_TIMEOUT )
        ;
    /* 等待传输结束 */
    while ( HAL_I2C_GetState( &hi2c2 ) != HAL_I2C_STATE_READY ) {
    }
    return status;
}

/* USER CODE END 1 */
