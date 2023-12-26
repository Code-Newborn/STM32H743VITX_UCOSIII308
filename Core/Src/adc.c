/**
 ******************************************************************************
 * @file    adc.c
 * @brief   This file provides code for the configuration
 *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
// 参考：https://blog.csdn.net/weixin_46461874/article/details/128686185
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;

/* ADC1 init function */
void MX_ADC1_Init( void ) {

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_MultiModeTypeDef   multimode = { 0 };
    ADC_ChannelConfTypeDef sConfig   = { 0 };

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */
    /** Common config
     */
    hadc1.Instance                      = ADC1;
    hadc1.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV4;
    hadc1.Init.Resolution               = ADC_RESOLUTION_16B;
    hadc1.Init.ScanConvMode             = ADC_SCAN_ENABLE;
    hadc1.Init.EOCSelection             = ADC_EOC_SEQ_CONV;
    hadc1.Init.LowPowerAutoWait         = DISABLE;
    hadc1.Init.ContinuousConvMode       = DISABLE;
    hadc1.Init.NbrOfConversion          = 2;
    hadc1.Init.DiscontinuousConvMode    = ENABLE;
    hadc1.Init.NbrOfDiscConversion      = 2;
    hadc1.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc1.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    hadc1.Init.OversamplingMode         = DISABLE;
    if ( HAL_ADC_Init( &hadc1 ) != HAL_OK ) {
        Error_Handler();
    }
    /** Configure the ADC multi-mode
     */
    multimode.Mode = ADC_MODE_INDEPENDENT;
    if ( HAL_ADCEx_MultiModeConfigChannel( &hadc1, &multimode ) != HAL_OK ) {
        Error_Handler();
    }
    /** Configure Regular Channel
     */
    sConfig.Channel                = ADC_CHANNEL_5;
    sConfig.Rank                   = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime           = ADC_SAMPLETIME_2CYCLES_5;
    sConfig.SingleDiff             = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber           = ADC_OFFSET_NONE;
    sConfig.Offset                 = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    if ( HAL_ADC_ConfigChannel( &hadc1, &sConfig ) != HAL_OK ) {
        Error_Handler();
    }
    /** Configure Regular Channel
     */
    sConfig.Channel = ADC_CHANNEL_10;
    sConfig.Rank    = ADC_REGULAR_RANK_2;
    if ( HAL_ADC_ConfigChannel( &hadc1, &sConfig ) != HAL_OK ) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}
/* ADC3 init function */
void MX_ADC3_Init( void ) {

    /* USER CODE BEGIN ADC3_Init 0 */

    /* USER CODE END ADC3_Init 0 */

    ADC_ChannelConfTypeDef sConfig = { 0 };

    /* USER CODE BEGIN ADC3_Init 1 */

    /* USER CODE END ADC3_Init 1 */
    /** Common config
     */
    hadc3.Instance                      = ADC3;
    hadc3.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV4;
    hadc3.Init.Resolution               = ADC_RESOLUTION_16B;
    hadc3.Init.ScanConvMode             = ADC_SCAN_ENABLE;
    hadc3.Init.EOCSelection             = ADC_EOC_SEQ_CONV;
    hadc3.Init.LowPowerAutoWait         = DISABLE;
    hadc3.Init.ContinuousConvMode       = DISABLE;
    hadc3.Init.NbrOfConversion          = 2;
    hadc3.Init.DiscontinuousConvMode    = ENABLE;
    hadc3.Init.NbrOfDiscConversion      = 2;
    hadc3.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
    hadc3.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc3.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
    hadc3.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    hadc3.Init.OversamplingMode         = DISABLE;
    if ( HAL_ADC_Init( &hadc3 ) != HAL_OK ) {
        Error_Handler();
    }
    /** Configure Regular Channel
     */
    sConfig.Channel                = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank                   = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime           = ADC_SAMPLETIME_810CYCLES_5;
    sConfig.SingleDiff             = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber           = ADC_OFFSET_NONE;
    sConfig.Offset                 = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    if ( HAL_ADC_ConfigChannel( &hadc3, &sConfig ) != HAL_OK ) {
        Error_Handler();
    }
    /** Configure Regular Channel
     */
    sConfig.Channel      = ADC_CHANNEL_VREFINT;
    sConfig.Rank         = ADC_REGULAR_RANK_2;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
    if ( HAL_ADC_ConfigChannel( &hadc3, &sConfig ) != HAL_OK ) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC3_Init 2 */

    /* USER CODE END ADC3_Init 2 */
}

void HAL_ADC_MspInit( ADC_HandleTypeDef* adcHandle ) {

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if ( adcHandle->Instance == ADC1 ) {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* ADC1 clock enable */
        __HAL_RCC_ADC12_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PC0     ------> ADC1_INP10
        PB1     ------> ADC1_INP5
        */
        GPIO_InitStruct.Pin  = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

        GPIO_InitStruct.Pin  = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    } else if ( adcHandle->Instance == ADC3 ) {
        /* USER CODE BEGIN ADC3_MspInit 0 */

        /* USER CODE END ADC3_MspInit 0 */
        /* ADC3 clock enable */
        __HAL_RCC_ADC3_CLK_ENABLE();
        /* USER CODE BEGIN ADC3_MspInit 1 */

        /* USER CODE END ADC3_MspInit 1 */
    }
}

void HAL_ADC_MspDeInit( ADC_HandleTypeDef* adcHandle ) {

    if ( adcHandle->Instance == ADC1 ) {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC12_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PC0     ------> ADC1_INP10
        PB1     ------> ADC1_INP5
        */
        HAL_GPIO_DeInit( GPIOC, GPIO_PIN_0 );

        HAL_GPIO_DeInit( GPIOB, GPIO_PIN_1 );

        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    } else if ( adcHandle->Instance == ADC3 ) {
        /* USER CODE BEGIN ADC3_MspDeInit 0 */

        /* USER CODE END ADC3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC3_CLK_DISABLE();
        /* USER CODE BEGIN ADC3_MspDeInit 1 */

        /* USER CODE END ADC3_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
