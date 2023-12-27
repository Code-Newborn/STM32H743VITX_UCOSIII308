/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "adc.h"
#include "gpio.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config( void );
void PeriphCommonClock_Config( void );
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t TS_CAL1;
uint16_t TS_CAL2;
uint16_t VREFINT_CAL;
float    Temp_oC;
float    Vrefint;
float    AD1_5;
float    AD1_10;

uint16_t AD_value[ 2 ]  = { 0 };
uint16_t AD_value_[ 2 ] = { 0 };

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main( void ) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* Configure the peripherals common clocks */
    PeriphCommonClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();
    MX_ADC3_Init();
    /* USER CODE BEGIN 2 */
    if ( HAL_ADCEx_Calibration_Start( &hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED ) != HAL_OK ) {
        printf( "ADC1校准失败\r\n" );
    }

    if ( HAL_ADCEx_Calibration_Start( &hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED ) != HAL_OK ) {
        printf( "ADC3校准失败\r\n" );
    }

    VREFINT_CAL = *( __IO uint16_t* )( 0x1FF1E860 );
    TS_CAL1     = *( __IO uint16_t* )( 0x1FF1E820 );
    TS_CAL2     = *( __IO uint16_t* )( 0x1FF1E840 );
    printf( "VREFINT_CAL:%d\r\n", VREFINT_CAL );

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while ( 1 ) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        HAL_GPIO_TogglePin( LED_GPIO_Port, LED_Pin );
        // HAL_Delay(200);

        /* 根据参考手册给的公式计算温度值 */

        for ( uint8_t i = 0; i < 2; i++ ) {
            HAL_ADC_Start( &hadc3 );                      // 开启ADC3
            HAL_ADC_PollForConversion( &hadc3, 10 );      // 等待转换完成
            AD_value_[ i ] = HAL_ADC_GetValue( &hadc3 );  // 依次获取ADC转换值

            if ( i == 0 )
                Temp_oC = ( ( 110.0f - 30.0f ) / ( TS_CAL2 - TS_CAL1 ) ) * ( AD_value_[ 0 ] - TS_CAL1 ) + 30.0f;
            if ( i == 1 )
                Vrefint = ( 3.3f * VREFINT_CAL ) / AD_value_[ 1 ];
        }
        printf( "ADC3_TempSensor:\t%.3f\r\n", Temp_oC );  // 打印获取片内温度传感器值
        printf( "ADC3_VrefInt:\t%.3f v\r\n", Vrefint );   // 打印参考电压转换值
        printf( "\n" );

        for ( uint8_t i = 0; i < 2; i++ ) {
            HAL_ADC_Start( &hadc1 );                     // 开启ADC1
            HAL_ADC_PollForConversion( &hadc1, 50 );     // 等待转换完成
            AD_value[ i ] = HAL_ADC_GetValue( &hadc1 );  // 依次获取ADC转换值

            if ( i == 0 )
                AD1_5 = ( float )( 3.3f * VREFINT_CAL * AD_value[ 0 ] ) / ( AD_value_[ 1 ] * ( 65536 - 1 ) );
            if ( i == 1 )
                AD1_10 = ( float )( 3.3f * VREFINT_CAL * AD_value[ 1 ] ) / ( AD_value_[ 1 ] * ( 65536 - 1 ) );
        }
        // 打印获取电压采样值
        printf( "X_ADC1_INP5_DATA:\t%d\r\n", AD_value[ 0 ] );
        printf( "X_ADC1_INP5_VOLT:\t%.3f\r\n", AD1_5 );
        // 打印电压转换值 精度16bit 2^16 根据实时参考电压校正值
        printf( "X_ADC1_INP10_DATA:\t%d\r\n", AD_value[ 1 ] );
        printf( "Y_ADC1_INP10_VOLT:\t%.3f v\r\n", AD1_10 );
        printf( "\n" );

        HAL_Delay( 500 );
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config( void ) {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Supply configuration update enable
     */
    HAL_PWREx_ConfigSupply( PWR_LDO_SUPPLY );
    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE1 );

    while ( !__HAL_PWR_GET_FLAG( PWR_FLAG_VOSRDY ) ) {
    }
    /** Macro to configure the PLL clock source
     */
    __HAL_RCC_PLL_PLLSOURCE_CONFIG( RCC_PLLSOURCE_HSI );
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = 4;
    RCC_OscInitStruct.PLL.PLLN            = 50;
    RCC_OscInitStruct.PLL.PLLP            = 2;
    RCC_OscInitStruct.PLL.PLLQ            = 2;
    RCC_OscInitStruct.PLL.PLLR            = 2;
    RCC_OscInitStruct.PLL.PLLRGE          = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL       = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN        = 0;
    if ( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK ) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if ( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_2 ) != HAL_OK ) {
        Error_Handler();
    }
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config( void ) {
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    /** Initializes the peripherals clock
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL2.PLL2M           = 4;
    PeriphClkInitStruct.PLL2.PLL2N           = 9;
    PeriphClkInitStruct.PLL2.PLL2P           = 2;
    PeriphClkInitStruct.PLL2.PLL2Q           = 2;
    PeriphClkInitStruct.PLL2.PLL2R           = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE         = RCC_PLL2VCIRANGE_3;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL      = RCC_PLL2VCOMEDIUM;
    PeriphClkInitStruct.PLL2.PLL2FRACN       = 3072;
    PeriphClkInitStruct.AdcClockSelection    = RCC_ADCCLKSOURCE_PLL2;
    if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK ) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler( void ) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while ( 1 ) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed( uint8_t* file, uint32_t line ) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
