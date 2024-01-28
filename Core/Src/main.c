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
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "ESP8266.h"
#include "cJSON/cJSON.h"
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

int str_len;

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

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USART3_UART_Init();
    /* USER CODE BEGIN 2 */
    delay_init( 400 );

    // 烧写固件版本 Ai-Thinker_ESP8266_DOUT_8Mbit_v1.5.4.1-a_20171130.bin
    ESP8266_Init();

    // ESP8266_StaTcpClient_UnvarnishTest_LedCtrl();  // 手机连接ESP8266时需断开手机流量网络
    ESP8266_ConnectWiFi();  // 连接WIFI

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while ( 1 ) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        // 私钥才能访问
        // 中文 https://api.seniverse.com/v3/weather/now.json?key=SNnoJymqbUTJCXYiX&location=beijing&language=zh-Hans&unit=c
        // 英文 GET https://api.seniverse.com/v3/weather/now.json?key=SNnoJymqbUTJCXYiX&location=zhaoqing&language=en&unit=c
        // 包含最后2字节转义符号共112+2=114字节
        char* request_url = "GET https://api.seniverse.com/v3/weather/now.json?key=SNnoJymqbUTJCXYiX&location=haerbin&language=zh-Hans&unit=c\r\n";
        str_len           = strlen( request_url );
        ESP8266_SendString( ENABLE, request_url, str_len, Single_ID_0 );

        char* weather_str = ESP8266_ReceiveString( ENABLE );

        printf( weather_str );  // 打印API返回数据
        printf( "\r\n\n" );
        printf( "【解析数据】 \r\n" );

        // cJson解析库使用 https://zhuanlan.zhihu.com/p/54574542

        cJSON* root;
        cJSON* results;
        cJSON* last_update;
        cJSON *loc_json, *now_json;
        char * loc_tmp, *weather_tmp, *update_tmp;

        root = cJSON_Parse( ( const char* )weather_str );
        if ( root ) {
            // printf( "JSON格式正确:\n%s\n\n", cJSON_Print( root ) );  // 输出json字符串
            results = cJSON_GetObjectItem( root, "results" );
            results = cJSON_GetArrayItem( results, 0 );
            if ( results ) {
                loc_json = cJSON_GetObjectItem( results, "location" );  // 得到location键对应的值，是一个对象
                if ( loc_json ) {
                    loc_tmp = cJSON_GetObjectItem( loc_json, "id" )->valuestring;
                    printf( "城市ID:%s\r\n", loc_tmp );
                    loc_tmp = cJSON_GetObjectItem( loc_json, "name" )->valuestring;
                    printf( "城市名称:%s\r\n", loc_tmp );
                    loc_tmp = cJSON_GetObjectItem( loc_json, "timezone" )->valuestring;
                    printf( "城市时区:%s\r\n", loc_tmp );
                } else
                    printf( "daily json格式错误\r\n" );

                now_json = cJSON_GetObjectItem( results, "now" );
                if ( now_json ) {
                    weather_tmp = cJSON_GetObjectItem( now_json, "text" )->valuestring;
                    printf( "天气:%s\r\n", weather_tmp );
                    weather_tmp = cJSON_GetObjectItem( now_json, "code" )->valuestring;
                    printf( "天气代码:%s\r\n", weather_tmp );
                    weather_tmp = cJSON_GetObjectItem( now_json, "temperature" )->valuestring;
                    printf( "温度:%s\r\n", weather_tmp );
                } else
                    printf( "daily json格式错误\r\n" );

                last_update = cJSON_GetObjectItem( results, "last_update" );
                update_tmp  = last_update->valuestring;
                if ( last_update ) {
                    printf( "更新时间:%s\r\n", update_tmp );
                }
            } else {
                printf( "results格式错误:%s\r\n", cJSON_GetErrorPtr() );
            }
        } else {
            printf( "JSON格式错误\r\n" );
        }

        cJSON_Delete( root );

        delay_ms( 5000 );  // 延时时间加大，连续获取
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
