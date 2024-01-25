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
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DHT11.h"
#include "delay.h"
#include "lcd.h"
#include "pic.h"
#include "sys.h"

#include "lvgl.h"
#include "porting/lv_port_disp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t Pin_state = 0;
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

void lv_ex_label( void ) {
    char*     github_addr = "lvgl_test";
    lv_obj_t* label       = lv_label_create( lv_scr_act() );
    lv_label_set_recolor( label, true );
    lv_label_set_long_mode( label, LV_LABEL_LONG_SCROLL_CIRCULAR ); /*Circular scroll*/
    lv_obj_set_width( label, 120 );
    lv_label_set_text_fmt( label, "test", github_addr );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 10 );

    lv_obj_t* label2 = lv_label_create( lv_scr_act() );
    lv_label_set_recolor( label2, true );
    lv_label_set_long_mode( label2, LV_LABEL_LONG_SCROLL_CIRCULAR ); /*Circular scroll*/
    lv_obj_set_width( label2, 120 );
    lv_label_set_text_fmt( label2, "123" );
    lv_obj_align( label2, LV_ALIGN_CENTER, 0, -10 );
}

void lv_example_led_1( void ) {
    /*Create a LED and switch it OFF*/
    lv_obj_t* led1 = lv_led_create( lv_scr_act() );
    lv_obj_align( led1, LV_ALIGN_CENTER, -80, 0 );
    lv_led_off( led1 );

    /*Copy the previous LED and set a brightness*/
    lv_obj_t* led2 = lv_led_create( lv_scr_act() );
    lv_obj_align( led2, LV_ALIGN_CENTER, 0, 0 );
    lv_led_set_brightness( led2, 150 );
    lv_led_set_color( led2, lv_palette_main( LV_PALETTE_RED ) );

    /*Copy the previous LED and switch it ON*/
    lv_obj_t* led3 = lv_led_create( lv_scr_act() );
    lv_obj_align( led3, LV_ALIGN_CENTER, 80, 0 );
    lv_led_on( led3 );
}

void Lvgl_Lable_Demo( void ) {
    lv_obj_t* scr    = lv_scr_act();
    lv_obj_t* label1 = lv_label_create( scr );
    lv_label_set_long_mode( label1, LV_LABEL_LONG_WRAP );
    lv_label_set_recolor( label1, true );
    lv_label_set_text( label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label, align the lines to the center "
                               "and wrap long text automatically." );
    lv_obj_set_width( label1, 150 );
    lv_obj_set_style_text_align( label1, LV_TEXT_ALIGN_CENTER, 0 );
    lv_obj_align( label1, LV_ALIGN_CENTER, 0, -40 );

    lv_obj_t* label2 = lv_label_create( scr );
    lv_label_set_long_mode( label2, LV_LABEL_LONG_SCROLL_CIRCULAR );
    lv_obj_set_width( label2, 150 );
    lv_label_set_text( label2, "It is a circularly scrolling text. " );
    lv_obj_align( label2, LV_ALIGN_CENTER, 0, 40 );
}

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
    MX_SPI1_Init();
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */
    delay_init( 400 );

    LCD_Init();  // LCD初始化
    HAL_TIM_Base_Start_IT( &htim3 );
    // LCD_Fill( 0, 0, LCD_W, LCD_H, WHITE );

    lv_init();
    lv_port_disp_init();

    lv_ex_label();
    Lvgl_Lable_Demo();
    lv_example_led_1();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while ( 1 ) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        lv_task_handler();
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

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE0 );

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
