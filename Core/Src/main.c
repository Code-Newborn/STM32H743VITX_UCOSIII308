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
#include "gpio.h"
#include "spi.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Ink_bmp.h"
#include "Ink_oled.h"
#include "delay.h"

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t Image_BW[ 4796 ];

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

    /* USER CODE BEGIN SysInit */
    delay_init( 400 );
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_SPI1_Init();
    /* USER CODE BEGIN 2 */

    uint8_t t = ' ';
    OLED_GUIInit();
    Paint_NewImage( Image_BW, OLED_W, OLED_H, ROTATE_0, WHITE );
    OLED_Clear( WHITE );

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while ( 1 ) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        OLED_GUIInit();
        OLED_ShowPicture( 0, 0, 296, 128, gImage_1, BLACK );
        OLED_Display( Image_BW );
        delay_ms( 1000 );
        delay_ms( 1000 );
        delay_ms( 1000 );
        OLED_Clear( WHITE );
        OLED_GUIInit();
        OLED_ShowChinese( 61, 0, 0, 16, BLACK );    // 中
        OLED_ShowChinese( 77, 0, 1, 16, BLACK );    // 景
        OLED_ShowChinese( 93, 0, 2, 16, BLACK );    // 园
        OLED_ShowChinese( 109, 0, 3, 16, BLACK );   // 电
        OLED_ShowChinese( 125, 0, 4, 16, BLACK );   // 子
        OLED_ShowChinese( 141, 0, 5, 16, BLACK );   // 技
        OLED_ShowChinese( 157, 0, 6, 16, BLACK );   // 术
        OLED_ShowChinese( 173, 0, 7, 16, BLACK );   // 有
        OLED_ShowChinese( 189, 0, 8, 16, BLACK );   // 限
        OLED_ShowChinese( 205, 0, 9, 16, BLACK );   // 公
        OLED_ShowChinese( 221, 0, 10, 16, BLACK );  // 司
        OLED_ShowString( 108, 20, "2014/05/01", 16, BLACK );
        OLED_ShowString( 80, 40, "ASCII:", 16, BLACK );
        OLED_ShowString( 157, 40, "CODE:", 16, BLACK );
        OLED_ShowChar( 131, 40, t, 16, BLACK );
        OLED_ShowNum( 198, 40, t, 3, 16, BLACK );
        t++;
        if ( t > '~' )
            t = ' ';
        OLED_ShowString( 33, 56, "Welcome to 2.90-inch E-paper", 16, BLACK );
        OLED_ShowString( 49, 73, "with 296 x 128 resolution", 16, BLACK );
        OLED_DrawLine( 1, 89, 296, 89, BLACK );
        OLED_ShowString( 53, 90, "Zhongjingyuan Electronic", 16, BLACK );
        OLED_ShowString( 75, 106, "Technology Co.,Ltd.", 16, BLACK );
        OLED_Display( Image_BW );
        delay_ms( 1000 );
        delay_ms( 1000 );
        delay_ms( 1000 );
        OLED_GUIInit();
        OLED_DrawRectangle( 1, 1, 296, 128, BLACK, 0 );
        OLED_Display( Image_BW );
        delay_ms( 1000 );
        delay_ms( 1000 );
        delay_ms( 1000 );
        OLED_Clear( WHITE );
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
    RCC_OscInitStruct.PLL.PLLQ            = 4;
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
