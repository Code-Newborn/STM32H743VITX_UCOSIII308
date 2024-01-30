/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
 *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "time.h"
#include "DHT11/DHT11.h"
#include "main.h"
#include "millis.h"
#include "typedefs.h"
/* USER CODE END 0 */

TIM_HandleTypeDef htim3;

/* TIM3 init function */
void MX_TIM3_Init( void ) {

    /* USER CODE BEGIN TIM3_Init 0 */

    /* USER CODE END TIM3_Init 0 */

    TIM_ClockConfigTypeDef  sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig      = { 0 };

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance               = TIM3;
    htim3.Init.Prescaler         = 20 - 1;
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.Period            = 10000 - 1;
    htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if ( HAL_TIM_Base_Init( &htim3 ) != HAL_OK ) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if ( HAL_TIM_ConfigClockSource( &htim3, &sClockSourceConfig ) != HAL_OK ) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if ( HAL_TIMEx_MasterConfigSynchronization( &htim3, &sMasterConfig ) != HAL_OK ) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */

    /* USER CODE END TIM3_Init 2 */
}

void HAL_TIM_Base_MspInit( TIM_HandleTypeDef* tim_baseHandle ) {

    if ( tim_baseHandle->Instance == TIM3 ) {
        /* USER CODE BEGIN TIM3_MspInit 0 */

        /* USER CODE END TIM3_MspInit 0 */
        /* TIM3 clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();

        /* TIM3 interrupt Init */
        HAL_NVIC_SetPriority( TIM3_IRQn, 1, 0 );
        HAL_NVIC_EnableIRQ( TIM3_IRQn );
        /* USER CODE BEGIN TIM3_MspInit 1 */

        /* USER CODE END TIM3_MspInit 1 */
    }
}

void HAL_TIM_Base_MspDeInit( TIM_HandleTypeDef* tim_baseHandle ) {

    if ( tim_baseHandle->Instance == TIM3 ) {
        /* USER CODE BEGIN TIM3_MspDeInit 0 */

        /* USER CODE END TIM3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();

        /* TIM3 interrupt Deinit */
        HAL_NVIC_DisableIRQ( TIM3_IRQn );
        /* USER CODE BEGIN TIM3_MspDeInit 1 */

        /* USER CODE END TIM3_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

extern char imu_run;
uint16_t    t3_count;
void        HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef* htim ) {
    // __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // 手动清除中断标志位
    if ( htim == &htim3 ) {
        // wifi_wait_data_hander();
        milliseconds++;
        update = true;

        // 温湿度任务
        if ( Task_Delay[ 0 ] == 0 ) {
            if ( DHT11_run ) {
                DHT11();
            }
            Task_Delay[ 0 ] = 100;  // 采样间隔100ms
        }

        // 更新任务
        for ( uint8_t i = 0; i < NumOfTask; i++ ) {
            if ( Task_Delay[ i ] != 0 ) {
                Task_Delay[ i ]--;
            }
        }

#ifdef RTC_SRC
        update = true;
#else

        if ( milliseconds % 1000 == 0 ) {
            update = true;
            ++timeDate.time.secs;  // 每秒钟标志位+1;
        }

        if ( milliseconds % 5 == 0 ) {
            if ( imu_run ) {
                // mpu_dmp_get_data(&pitch, &roll, &yaw);
            }
        }

        // if ( milliseconds % 10 == 0 ) {  // 间隔过短影响界面刷新
        //     if ( DHT11_run ) {
        //         DHT11();  // 获取温湿度数据
        //     }
        // }

#endif
    }
}

/* USER CODE END 1 */
