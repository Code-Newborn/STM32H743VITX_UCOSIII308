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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
#include "delay.h"
#include "OLED_SSD1306.h"
#include "OLED_SSD1306_BMP.h"
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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
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
  /* USER CODE BEGIN 2 */
  // uint8_t t;
  OLED_Init(); // OLED初始化
  OLED_Clear();
  OLED_ShowChinese(0, 0, 0, 16);
  OLED_ShowChinese(34, 0, 1, 16);
  OLED_ShowChinese(68, 0, 2, 16);
  OLED_ShowChinese(0, 32, 3, 32);
  OLED_ShowChinese(34, 32, 4, 32);
  OLED_ShowChinese(68, 32, 5, 32);
  OLED_Refresh();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // =============== 灯LED翻转闪烁 ===============
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HAL_Delay(200);

    // =============== 显示屏OLED 显示 BEGIN===============
    // OLED_ShowPicture(0, 0, 128, 8, BMP1);
    // HAL_Delay(1000);
    // OLED_ShowPicture(0, 0, 128, 8, BMP2);
    // OLED_Clear();
    // OLED_ShowChinese(0, 0, 0, 16);  // 中
    // OLED_ShowChinese(18, 0, 1, 16); // 景
    // OLED_ShowChinese(36, 0, 2, 16); // 园
    // OLED_ShowChinese(54, 0, 3, 16); // 电
    // OLED_ShowChinese(72, 0, 4, 16); // 子
    // OLED_ShowChinese(90, 0, 5, 16); // 科
    // OLED_Refresh();
    // HAL_Delay(1000);
    // OLED_ShowChinese(0, 0, 0, 16);   // 中
    // OLED_ShowChinese(18, 0, 1, 16);  // 景
    // OLED_ShowChinese(36, 0, 2, 16);  // 园
    // OLED_ShowChinese(54, 0, 3, 16);  // 电
    // OLED_ShowChinese(72, 0, 4, 16);  // 子
    // OLED_ShowChinese(90, 0, 5, 16);  // 科
    // OLED_ShowChinese(108, 0, 6, 16); // 技
    // OLED_ShowString(8, 16, "ZHONGJINGYUAN", 16);
    // OLED_ShowString(20, 32, "2014/05/01", 16);
    // OLED_ShowString(0, 48, "ASCII:", 16);
    // OLED_ShowString(63, 48, "CODE:", 16);
    // OLED_ShowChar(48, 48, t, 16); // 显示ASCII字符
    // t++;
    // if (t > '~')
    //   t = ' ';
    // OLED_ShowNum(103, 48, t, 3, 16);
    // OLED_Refresh();
    // HAL_Delay(2000);
    // OLED_Clear();
    // OLED_ShowChinese(0, 0, 0, 16);   // 16*16 中
    // OLED_ShowChinese(16, 0, 0, 24);  // 24*24 中
    // OLED_ShowChinese(24, 20, 0, 32); // 32*32 中
    // OLED_ShowChinese(64, 0, 0, 64);  // 64*64 中
    // OLED_Refresh();
    // HAL_Delay(2000);
    // OLED_Clear();
    // OLED_ShowString(0, 0, "ABC", 12);  // 6*12 “ABC”
    // OLED_ShowString(0, 12, "ABC", 16); // 8*16 “ABC”
    // OLED_ShowString(0, 28, "ABC", 24); // 12*24 “ABC”
    // OLED_Refresh();
    // HAL_Delay(2000);
    // OLED_ScrollDisplay(11, 4);
    // =============== 显示屏OLED 显示 END ===============
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
   */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }
  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
