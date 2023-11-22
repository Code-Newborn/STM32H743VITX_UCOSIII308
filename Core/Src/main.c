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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
#include "delay.h"
#include "OLED_SSD1306.h"
#include "OLED_SSD1306_BMP.h"
#include "DHT11.h"

#include "draw_api.h"
#include "test.h"
#include "OLED_SSD1306.h"
#include "common.h"
#include "oled_driver.h"
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
enum
{
  LA = 262,
  LB = 294,
  LC = 330,
  LD = 349,
  LE = 392,
  LF = 440,
  LG = 494,

  MA = 523,
  MB = 578,
  MC = 659,
  MD = 698,
  ME = 784,
  MF = 880,
  MG = 988,

  HA = 1064,
  HB = 1175,
  HC = 1318,
  HD = 1397,
  HE = 1568,
  HF = 1760,
  HG = 1976
};

const uint32_t STAY[] = {

    // 5353
    ME << 16 | 250,
    MC << 16 | 250,
    ME << 16 | 250,
    MC << 16 | 250,
    // 222321
    MB << 16 | 125,
    MB << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,
    MB << 16 | 125,
    MA << 16 | 250,
    // 7115
    LG << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 125,
    ME << 16 | 500,
    // 177777111
    MA << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 250,
    // 1715
    MA << 16 | 125,
    LG << 16 | 125,
    MA << 16 | 125,
    ME << 16 | 500,
    // 177777111
    MA << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 250,
    // 1715
    MA << 16 | 125,
    LG << 16 | 125,
    MA << 16 | 125,
    ME << 16 | 500,
    // 177777111
    MA << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    LG << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 250,
    // 1715
    MA << 16 | 125,
    LG << 16 | 125,
    MA << 16 | 125,
    ME << 16 | 500,
    // 71275
    LG << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MG << 16 | 125,
    ME << 16 | 500,

    TONE_REPEAT

}; // 旋律

const uint32_t TUNE[] = {

    LF << 16 | 250,
    LC << 16 | 250,
    HF << 16 | 250,
    MC << 16 | 250,

    LD << 16 | 250,
    MA << 16 | 250,
    MD << 16 | 250,
    MA << 16 | 250,

    LE << 16 | 250,
    MB << 16 | 250,
    ME << 16 | 250,
    MB << 16 | 250,

    MA << 16 | 250,
    ME << 16 | 250,
    HA << 16 | 250,
    ME << 16 | 250,

    LF << 16 | 250,
    LC << 16 | 250,
    HF << 16 | 250,
    MC << 16 | 250,

    LD << 16 | 250,
    MA << 16 | 250,
    MD << 16 | 250,
    MA << 16 | 250,

    LE << 16 | 250,
    MB << 16 | 250,
    ME << 16 | 250,
    MB << 16 | 250,
    // 1 5123
    MA << 16 | 500,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,
    // 2111
    MB << 16 | 250,
    MA << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 500,

    TONE_PAUSE << 16 | 125,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,

    // 212233
    MB << 16 | 250,
    MA << 16 | 125,
    MB << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 250,
    MC << 16 | 125,

    // 35123
    MC << 16 | 500,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,

    // 2111
    MB << 16 | 250,
    MA << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 500,

    // 05123
    TONE_PAUSE << 16 | 125,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,
    // 212253
    MB << 16 | 250,
    MA << 16 | 125,
    MB << 16 | 125,
    MB << 16 | 125,
    ME << 16 | 250,
    MC << 16 | 125,

    // 334
    MC << 16 | 500,
    MC << 16 | 250,
    MD << 16 | 250,

    // 55555
    ME << 16 | 250,
    ME << 16 | 125,
    ME << 16 | 125,
    ME << 16 | 250,
    ME << 16 | 250,

    // 531134
    ME << 16 | 250,
    MC << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 125,
    MC << 16 | 250,
    MD << 16 | 125,

    // 55555
    ME << 16 | 250,
    ME << 16 | 125,
    ME << 16 | 125,
    ME << 16 | 250,
    ME << 16 | 250,
    // 531 112
    ME << 16 | 250,
    MC << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 250,
    MA << 16 | 125,
    MB << 16 | 125,

    // 33333
    MC << 16 | 250,
    MC << 16 | 125,
    MC << 16 | 125,
    MC << 16 | 250,
    MC << 16 | 250,
    // 366321
    MC << 16 | 250,
    LF << 16 | 250,
    MF << 16 | 125,
    MC << 16 | 125,
    MB << 16 | 125,
    LA << 16 | 250,

    // 20
    MB << 16 | 500,
    TONE_PAUSE << 16 | 125,

    // 05123
    TONE_PAUSE << 16 | 125,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,
    // 2111
    MB << 16 | 250,
    MA << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 500,
    // 05123
    TONE_PAUSE << 16 | 125,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,

    // 212233
    MB << 16 | 250,
    MA << 16 | 125,
    MB << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 250,
    MC << 16 | 125,

    // 35123
    MC << 16 | 500,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,
    // 2111
    MB << 16 | 250,
    MA << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 500,
    // 05123
    TONE_PAUSE << 16 | 125,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,
    // 212253
    MB << 16 | 250,
    MA << 16 | 125,
    MB << 16 | 125,
    MB << 16 | 125,
    ME << 16 | 250,
    MC << 16 | 125,

    // 334
    MC << 16 | 500,
    MC << 16 | 250,
    MD << 16 | 250,

    // 55555
    ME << 16 | 250,
    ME << 16 | 125,
    ME << 16 | 125,
    ME << 16 | 250,
    ME << 16 | 250,

    // 531134
    ME << 16 | 250,
    MC << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 125,
    MC << 16 | 250,
    MD << 16 | 125,

    // 55555
    ME << 16 | 250,
    ME << 16 | 125,
    ME << 16 | 125,
    ME << 16 | 250,
    ME << 16 | 250,
    // 531 112
    ME << 16 | 250,
    MC << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 250,
    MA << 16 | 125,
    MB << 16 | 125,

    // 33333
    MC << 16 | 250,
    MC << 16 | 125,
    MC << 16 | 125,
    MC << 16 | 250,
    MC << 16 | 250,
    // 363216
    MC << 16 | 500,
    MF << 16 | 125,
    MC << 16 | 125,
    MB << 16 | 125,
    MA << 16 | 125,
    LF << 16 | 125,

    // 10
    MA << 16 | 500,
    TONE_PAUSE << 16 | 250,
    // 0
    // TONE_PAUSE<<16 | 3000,

    // 55555
    ME << 16 | 250,
    ME << 16 | 125,
    ME << 16 | 125,
    ME << 16 | 250,
    ME << 16 | 250,

    // 531134
    ME << 16 | 250,
    MC << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 125,
    MC << 16 | 250,
    MD << 16 | 125,

    // 55555
    ME << 16 | 250,
    ME << 16 | 125,
    ME << 16 | 125,
    ME << 16 | 250,
    ME << 16 | 250,
    // 531 112
    ME << 16 | 250,
    MC << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 250,
    MA << 16 | 125,
    MB << 16 | 125,

    // 33333
    MC << 16 | 250,
    MC << 16 | 125,
    MC << 16 | 125,
    MC << 16 | 250,
    MC << 16 | 250,
    // 366321
    MC << 16 | 250,
    LF << 16 | 250,
    MF << 16 | 125,
    MC << 16 | 125,
    MB << 16 | 125,
    LA << 16 | 250,

    // 20
    MB << 16 | 500,
    TONE_PAUSE << 16 | 125,

    // 05123
    TONE_PAUSE << 16 | 125,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,
    // 2111
    MB << 16 | 250,
    MA << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 500,
    // 05123
    TONE_PAUSE << 16 | 125,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,

    // 212233
    MB << 16 | 250,
    MA << 16 | 125,
    MB << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 250,
    MC << 16 | 125,

    // 35123
    MC << 16 | 500,
    LE << 16 | 125,
    MA << 16 | 125,
    MB << 16 | 125,
    MC << 16 | 125,

    // 2111
    MB << 16 | 250,
    MA << 16 | 125,
    MA << 16 | 125,
    MA << 16 | 500,

    TONE_PAUSE << 16 | 2000,

    TONE_REPEAT

}; // 旋律
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void c_setup()
{

  // SystemInit();
  // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组2
  // delay_init(168);          //初始化延时函数
  // #if COMPILE_UART
  //   uart_init(UART_BAUD); // 初始化串口波特率为115200
  //                         // printf("begun");
  // #endif

  buttons_init();
  // Yaogan_Init();
  LCD_Init(); // 初始化OLED接口

  // Usart2_Init(115200);
  // WiFi_ResetIO_Init();
  // millis_init();

  // console_log(200, "TEST:");
  // console_log(200, "1\r\n23\r\n456\n789a\nbcdef\nghijkl\nmnopqrs\ntuvwxyz~\n!@#$^&*()\n`-=_+[]\\|/");
  // console_log(200, "\n");
  // console_log(120, "--- welcome !---");
  // console_log(10, "  _   _    _    ");
  // console_log(10, " | | | |  |_|   ");
  // console_log(10, " | |_| |   _    ");
  // console_log(10, " |  _  |  | |   ");
  // console_log(10, " | | | |  | |   ");
  // console_log(10, " |_| |_|  |_|   ");
  // console_log(10, "--press button--");
  // while (KEY0)
  //   ;
  // console_log(1, "reset wifi");
  // // RESET_IO(0);   // 复位IO拉低电平
  // delay_ms(500); // 延时500ms
  // // RESET_IO(1);
  // delay_ms(1000);
  // // WiFi_send("AT\r\n");
  // console_log(1, "send AT ");
  // delay_ms(500);
  // WiFi_send("AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASS);
  // console_log(1, "%s", Usart2_RxBuff);
  // console_log(1, "%s", &Data_buff[2]);

  // char i = 0;
  // do
  // {
  //   i = MPU_Init();
  //   // printf("init1:%d",i);
  //   console_log(500, "init1 :%d", i);
  // } while (i & KEY0);

  // do
  // {
  //   i = mpu_dmp_init();
  //   // printf("init2:%d",i);
  //   console_log(500, "init2 :%d", i);
  // } while (i & KEY0);

  // console_log(500, "start !");
  milliseconds = 0;

  memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);

  appconfig_init();
  // led_init(); // 初始化LED
  // buzzer_init();
  // global_init();

  alarm_init(); // 无法储存闹钟，每次重启以后需要自定义

  time_init();
  // rtc_init();
  // pwrmgr_init();
  // Set watchface
  display_set(watchface_normal);

  display_load(); // 启动表盘
}

void c_loop()
{

  time_update();

  if (pwrmgr_userActive())
  {
    // battery_update();
    buttons_update();
  }

  // mpu_updata();

  // buzzer_update();
  // led_update();

#if COMPILE_STOPWATCH
  stopwatch_update();
#endif
  //  global_update();

  if (pwrmgr_userActive())
  {
    alarm_update();
    display_update();
  }

  pwrmgr_update();

  // 显示完成后清除缓冲区
  memset(&oledBuffer, 0xFF, FRAME_BUFFER_SIZE);
}
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
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  delay_init(400);
  // buttons_init();
  OLED_Init(); // OLED初始化
  // HAL_TIM_Base_Start_IT(&htim3);

  // c_setup(); // 初始化
  oledBuffer[12] = 0xFF;
  LCD_Flush();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // c_loop(); // 循环
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
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
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
