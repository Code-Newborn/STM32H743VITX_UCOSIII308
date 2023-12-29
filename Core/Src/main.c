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
#include "fmc.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "lcd.h"
#include "touch.h"
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

// 清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog( void ) {
    LCD_Clear( WHITE );                                          // 清屏
    POINT_COLOR = BLUE;                                          // 设置字体为蓝色
    LCD_ShowString( lcddev.width - 24, 0, 200, 16, 16, "RST" );  // 显示清屏区域
    POINT_COLOR = RED;                                           // 设置画笔蓝色
}
////////////////////////////////////////////////////////////////////////////////
// 电容触摸屏专有部分
// 画水平线
// x0,y0:坐标
// len:线长度
// color:颜色
void gui_draw_hline( uint16_t x0, uint16_t y0, uint16_t len, uint16_t color ) {
    if ( len == 0 )
        return;
    if ( ( x0 + len - 1 ) >= lcddev.width )
        x0 = lcddev.width - len - 1;  // 限制坐标范围
    if ( y0 >= lcddev.height )
        y0 = lcddev.height - 1;  // 限制坐标范围
    LCD_Fill( x0, y0, x0 + len - 1, y0, color );
}
// 画实心圆
// x0,y0:坐标
// r:半径
// color:颜色
void gui_fill_circle( uint16_t x0, uint16_t y0, uint16_t r, uint16_t color ) {
    uint32_t i;
    uint32_t imax  = ( ( uint32_t )r * 707 ) / 1000 + 1;
    uint32_t sqmax = ( uint32_t )r * ( uint32_t )r + ( uint32_t )r / 2;
    uint32_t x     = r;
    gui_draw_hline( x0 - r, y0, 2 * r, color );
    for ( i = 1; i <= imax; i++ ) {
        if ( ( i * i + x * x ) > sqmax )  // draw lines from outside
        {
            if ( x > imax ) {
                gui_draw_hline( x0 - i + 1, y0 + x, 2 * ( i - 1 ), color );
                gui_draw_hline( x0 - i + 1, y0 - x, 2 * ( i - 1 ), color );
            }
            x--;
        }
        // draw lines from inside (center)
        gui_draw_hline( x0 - x, y0 + i, 2 * x, color );
        gui_draw_hline( x0 - x, y0 - i, 2 * x, color );
    }
}
// 两个数之差的绝对值
// x1,x2：需取差值的两个数
// 返回值：|x1-x2|
uint16_t my_abs( uint16_t x1, uint16_t x2 ) {
    if ( x1 > x2 )
        return x1 - x2;
    else
        return x2 - x1;
}
// 画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
// size：线条的粗细程度
// color：线条的颜色
void lcd_draw_bline( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color ) {
    uint16_t t;
    int      xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int      incx, incy, uRow, uCol;
    if ( x1 < size || x2 < size || y1 < size || y2 < size )
        return;
    delta_x = x2 - x1;  // 计算坐标增量
    delta_y = y2 - y1;
    uRow    = x1;
    uCol    = y1;
    if ( delta_x > 0 )
        incx = 1;  // 设置单步方向
    else if ( delta_x == 0 )
        incx = 0;  // 垂直线
    else {
        incx    = -1;
        delta_x = -delta_x;
    }
    if ( delta_y > 0 )
        incy = 1;
    else if ( delta_y == 0 )
        incy = 0;  // 水平线
    else {
        incy    = -1;
        delta_y = -delta_y;
    }
    if ( delta_x > delta_y )
        distance = delta_x;  // 选取基本增量坐标轴
    else
        distance = delta_y;
    for ( t = 0; t <= distance + 1; t++ )  // 画线输出
    {
        gui_fill_circle( uRow, uCol, size, color );  // 画点
        xerr += delta_x;
        yerr += delta_y;
        if ( xerr > distance ) {
            xerr -= distance;
            uRow += incx;
        }
        if ( yerr > distance ) {
            yerr -= distance;
            uCol += incy;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
// 10个触控点的颜色(电容触摸屏用)
const uint16_t POINT_COLOR_TBL[ 10 ] = { RED, GREEN, BLUE, BROWN, GRED, BRED, GBLUE, LIGHTBLUE, BRRED, GRAY };
// 电阻触摸屏测试函数
void rtp_test( void ) {
    // uint8_t key;
    // uint8_t i = 0;
    while ( 1 ) {
        // key = KEY_Scan( 0 );
        tp_dev.scan( 0 );
        if ( tp_dev.sta & TP_PRES_DOWN )  // 触摸屏被按下
        {
            if ( tp_dev.x[ 0 ] < lcddev.width && tp_dev.y[ 0 ] < lcddev.height ) {
                if ( tp_dev.x[ 0 ] > ( lcddev.width - 24 ) && tp_dev.y[ 0 ] < 16 )
                    Load_Drow_Dialog();  // 清除
                else
                    TP_Draw_Big_Point( tp_dev.x[ 0 ], tp_dev.y[ 0 ], RED );  // 画图
            }
        } else
            delay_ms( 10 );  // 没有按键按下的时候
        // if ( key == KEY0_PRES )  // KEY0按下,则执行校准程序
        // {
        //     LCD_Clear( WHITE );  // 清屏
        //     TP_Adjust();         // 屏幕校准
        //     TP_Save_Adjdata();
        //     Load_Drow_Dialog();
        // }
        // i++;
        // if ( i % 20 == 0 )
        //     LED0 = !LED0;
    }
}
// 电容触摸屏测试函数
void ctp_test( void ) {
    uint8_t t = 0;
    // uint8_t  i = 0;
    uint16_t lastpos[ 10 ][ 2 ];  // 最后一次的数据
    uint8_t  maxp = 5;
    if ( lcddev.id == 0X1018 )
        maxp = 10;
    while ( 1 ) {
        tp_dev.scan( 0 );
        for ( t = 0; t < maxp; t++ ) {
            if ( ( tp_dev.sta ) & ( 1 << t ) ) {
                if ( tp_dev.x[ t ] < lcddev.width && tp_dev.y[ t ] < lcddev.height ) {
                    if ( lastpos[ t ][ 0 ] == 0XFFFF ) {
                        lastpos[ t ][ 0 ] = tp_dev.x[ t ];
                        lastpos[ t ][ 1 ] = tp_dev.y[ t ];
                    }
                    lcd_draw_bline( lastpos[ t ][ 0 ], lastpos[ t ][ 1 ], tp_dev.x[ t ], tp_dev.y[ t ], 2, POINT_COLOR_TBL[ t ] );  // 画线
                    lastpos[ t ][ 0 ] = tp_dev.x[ t ];
                    lastpos[ t ][ 1 ] = tp_dev.y[ t ];
                    if ( tp_dev.x[ t ] > ( lcddev.width - 24 ) && tp_dev.y[ t ] < 20 ) {
                        Load_Drow_Dialog();  // 清除
                    }
                }
            } else
                lastpos[ t ][ 0 ] = 0XFFFF;
        }
        delay_ms( 5 );
        // i++;
        // if ( i % 20 == 0 )
        //     LED0 = !LED0;
    }
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
    MX_FMC_Init();
    MX_SPI2_Init();
    /* USER CODE BEGIN 2 */

    tp_dev.init();  // 触摸屏初始化
    POINT_COLOR = RED;
    LCD_ShowString( 30, 50, 200, 16, 16, "Apollo STM32f4/F7" );
    LCD_ShowString( 30, 70, 200, 16, 16, "TOUCH TEST" );
    LCD_ShowString( 30, 90, 200, 16, 16, "ATOM@ALIENTEK" );
    LCD_ShowString( 30, 110, 200, 16, 16, "2016/1/16" );
    if ( tp_dev.touchtype != 0XFF ) {
        LCD_ShowString( 30, 130, 200, 16, 16, "Press KEY0 to Adjust" );  // 电阻屏才显示
    }
    delay_ms( 1500 );
    Load_Drow_Dialog();

    if ( tp_dev.touchtype & 0X80 )
        ctp_test();  // 电容屏测试
    else
        rtp_test();  // 电阻屏测试

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while ( 1 ) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        HAL_GPIO_TogglePin( LED_GPIO_Port, LED_Pin );
        HAL_Delay( 200 );
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
