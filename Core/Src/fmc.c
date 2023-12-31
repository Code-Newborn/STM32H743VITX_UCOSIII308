/**
 ******************************************************************************
 * File Name          : FMC.c
 * Description        : This file provides code for the configuration
 *                      of the FMC peripheral.
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
#include "fmc.h"

/* USER CODE BEGIN 0 */
#include "delay.h"
#include "lcd.h"
#include "usart.h"

/* USER CODE END 0 */

SRAM_HandleTypeDef hsram1;

/* FMC initialization function */
void MX_FMC_Init( void ) {
    /* USER CODE BEGIN FMC_Init 0 */

    /* USER CODE END FMC_Init 0 */

    FMC_NORSRAM_TimingTypeDef Timing    = { 0 };
    FMC_NORSRAM_TimingTypeDef ExtTiming = { 0 };

    /* USER CODE BEGIN FMC_Init 1 */

    /* USER CODE END FMC_Init 1 */

    /** Perform the SRAM1 memory initialization sequence
     */
    hsram1.Instance = FMC_NORSRAM_DEVICE;
    hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram1.Init */
    hsram1.Init.NSBank             = FMC_NORSRAM_BANK1;
    hsram1.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
    hsram1.Init.MemoryType         = FMC_MEMORY_TYPE_SRAM;
    hsram1.Init.MemoryDataWidth    = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram1.Init.BurstAccessMode    = FMC_BURST_ACCESS_MODE_DISABLE;
    hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram1.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
    hsram1.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
    hsram1.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;
    hsram1.Init.ExtendedMode       = FMC_EXTENDED_MODE_ENABLE;
    hsram1.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram1.Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;
    hsram1.Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    hsram1.Init.WriteFifo          = FMC_WRITE_FIFO_ENABLE;
    hsram1.Init.PageSize           = FMC_PAGE_SIZE_NONE;
    /* Timing */
    Timing.AddressSetupTime      = 2;
    Timing.AddressHoldTime       = 15;
    Timing.DataSetupTime         = 16;
    Timing.BusTurnAroundDuration = 0;
    Timing.CLKDivision           = 16;
    Timing.DataLatency           = 17;
    Timing.AccessMode            = FMC_ACCESS_MODE_A;
    /* ExtTiming */
    ExtTiming.AddressSetupTime      = 4;
    ExtTiming.AddressHoldTime       = 15;
    ExtTiming.DataSetupTime         = 15;
    ExtTiming.BusTurnAroundDuration = 0;
    ExtTiming.CLKDivision           = 16;
    ExtTiming.DataLatency           = 17;
    ExtTiming.AccessMode            = FMC_ACCESS_MODE_A;

    if ( HAL_SRAM_Init( &hsram1, &Timing, &ExtTiming ) != HAL_OK ) {
        Error_Handler();
    }

    /* USER CODE BEGIN FMC_Init 2 */
    delay_ms( 50 );  // delay 50 ms

    // 尝试9341或者9488 ID的读取
    LCD_WR_REG( 0XD3 );
    lcddev.id = LCD_RD_DATA();  // dummy read
    lcddev.id = LCD_RD_DATA();  // 读到0X00
    lcddev.id = LCD_RD_DATA();  // 读取93或者94
    lcddev.id <<= 8;
    lcddev.id |= LCD_RD_DATA();                                // 读取41或者88
    if ( ( lcddev.id != 0X9341 ) && ( lcddev.id != 0X9488 ) )  // 非9341也非9488,尝试看看是不是NT35310
    {
        LCD_WR_REG( 0XD4 );
        lcddev.id = LCD_RD_DATA();  // dummy read
        lcddev.id = LCD_RD_DATA();  // 读回0X01
        lcddev.id = LCD_RD_DATA();  // 读回0X53
        lcddev.id <<= 8;
        lcddev.id |= LCD_RD_DATA();  // 这里读回0X10
        if ( lcddev.id != 0X5310 )   // 也不是NT35310,尝试看看是不是NT35510
        {
            LCD_WR_REG( 0XDA00 );
            lcddev.id = LCD_RD_DATA();  // 读回0X00
            LCD_WR_REG( 0XDB00 );
            lcddev.id = LCD_RD_DATA();  // 读回0X80
            lcddev.id <<= 8;
            LCD_WR_REG( 0XDC00 );
            lcddev.id |= LCD_RD_DATA();  // 读回0X00
            if ( lcddev.id == 0x8000 )
                lcddev.id = 0x5510;  // NT35510读回的ID是008000H,为方便区分,我们强制设置为5510
            if ( lcddev.id == 0x8200 )
                lcddev.id = 0x6812;  // RM68120读回的ID是681211H,为方便区分,我们强制设置为6812
            if ( lcddev.id == 0x4000 )
                lcddev.id = 0x8009;  // OTM8009A读回的ID是8000H,为方便区分,我们强制设置为8009

            if ( ( lcddev.id != 0X5510 ) && ( lcddev.id != 0x6812 ) && ( lcddev.id != 0x8009 ) )  // 也不是NT5510,OTM8009,RM68120,尝试看看是不是SSD1963
            {
                LCD_WR_REG( 0XA1 );
                lcddev.id = LCD_RD_DATA();
                lcddev.id = LCD_RD_DATA();  // 读回0X57
                lcddev.id <<= 8;
                lcddev.id |= LCD_RD_DATA();  // 读回0X61
                if ( lcddev.id == 0X5761 )
                    lcddev.id = 0X1963;  // SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963
            }
        }
    }
    printf( " LCD ID:%x\r\n", lcddev.id );  // 打印LCD ID

    if ( lcddev.id == 0X9341 )  // 9341初始化
    {
#if 1  // 2.8寸兼容屏ILI9341 P280QVO1HIR-YZ
        LCD_WR_REG( 0xCF );
        LCD_WR_DATA( 0x00 );
        LCD_WR_DATA( 0xc3 );
        LCD_WR_DATA( 0X30 );

        LCD_WR_REG( 0xED );
        LCD_WR_DATA( 0x64 );
        LCD_WR_DATA( 0x03 );
        LCD_WR_DATA( 0X12 );
        LCD_WR_DATA( 0X81 );

        LCD_WR_REG( 0xE8 );
        LCD_WR_DATA( 0x85 );
        LCD_WR_DATA( 0x00 );
        LCD_WR_DATA( 0x78 );  // 7a

        LCD_WR_REG( 0xCB );
        LCD_WR_DATA( 0x39 );
        LCD_WR_DATA( 0x2C );
        LCD_WR_DATA( 0x00 );
        LCD_WR_DATA( 0x34 );
        LCD_WR_DATA( 0x02 );

        LCD_WR_REG( 0xF7 );
        LCD_WR_DATA( 0x20 );

        LCD_WR_REG( 0xEA );
        LCD_WR_DATA( 0x00 );
        LCD_WR_DATA( 0x00 );

        LCD_WR_REG( 0xC0 );   // Power control
        LCD_WR_DATA( 0x1b );  // VRH[5:0]  21

        LCD_WR_REG( 0xC1 );   // Power control
        LCD_WR_DATA( 0x12 );  // SAP[2:0];BT[3:0]

        LCD_WR_REG( 0xC5 );
        LCD_WR_DATA( 0x32 );  // 32
        LCD_WR_DATA( 0x3C );

        LCD_WR_REG( 0xC7 );   // VCM control2
        LCD_WR_DATA( 0x9A );  // 9e

        LCD_WR_REG( 0x36 );   // Memory Access Control
        LCD_WR_DATA( 0x08 );  // 00 RGB ??

        LCD_WR_REG( 0x3A );
        LCD_WR_DATA( 0x55 );

        LCD_WR_REG( 0xB1 );
        LCD_WR_DATA( 0x00 );
        LCD_WR_DATA( 0x1b );  // 16

        LCD_WR_REG( 0xB6 );  // Display Function Control
        LCD_WR_DATA( 0x0A );
        LCD_WR_DATA( 0xA2 );

        LCD_WR_REG( 0xF6 );
        LCD_WR_DATA( 0x01 );
        LCD_WR_DATA( 0x30 );

        LCD_WR_REG( 0xF2 );  // 3Gamma Function Disable
        LCD_WR_DATA( 0x00 );

        LCD_WR_REG( 0x26 );  // Gamma curve selected
        LCD_WR_DATA( 0x01 );

        LCD_WR_REG( 0xE0 );  // Set Gamma
        LCD_WR_DATA( 0x0F );
        LCD_WR_DATA( 0x1d );
        LCD_WR_DATA( 0x1a );
        LCD_WR_DATA( 0x0a );
        LCD_WR_DATA( 0x0d );
        LCD_WR_DATA( 0x07 );
        LCD_WR_DATA( 0x49 );
        LCD_WR_DATA( 0X66 );
        LCD_WR_DATA( 0x3b );
        LCD_WR_DATA( 0x07 );
        LCD_WR_DATA( 0x11 );
        LCD_WR_DATA( 0x01 );
        LCD_WR_DATA( 0x09 );
        LCD_WR_DATA( 0x05 );
        LCD_WR_DATA( 0x04 );  // 04

        LCD_WR_REG( 0XE1 );  // Set Gamma
        LCD_WR_DATA( 0x00 );
        LCD_WR_DATA( 0x18 );
        LCD_WR_DATA( 0x1d );
        LCD_WR_DATA( 0x02 );
        LCD_WR_DATA( 0x0f );
        LCD_WR_DATA( 0x04 );
        LCD_WR_DATA( 0x36 );
        LCD_WR_DATA( 0x13 );
        LCD_WR_DATA( 0x4c );
        LCD_WR_DATA( 0x07 );
        LCD_WR_DATA( 0x13 );
        LCD_WR_DATA( 0x0f );
        LCD_WR_DATA( 0x2E );
        LCD_WR_DATA( 0x2f );
        LCD_WR_DATA( 0x05 );  // 0F

        LCD_WR_REG( 0x11 );
        delay_ms( 120 );
        LCD_WR_REG( 0x29 );
        delay_ms( 20 );
#endif
    }

    // 初始化完成以后,提速
    if ( lcddev.id == 0X9341 || lcddev.id == 0X5310 || lcddev.id == 0X5510 || lcddev.id == 0x8009 || lcddev.id == 0x6812 || lcddev.id == 0X1963
         || lcddev.id == 0X9488 )  // 如果是这几个IC,则设置WR时序为最快
    {
        // 重新配置写时序控制寄存器的时序
        ExtTiming.AddressSetupTime = 4;
        ExtTiming.DataSetupTime    = 4;
        FMC_NORSRAM_Extended_Timing_Init( hsram1.Extended, &ExtTiming, hsram1.Init.NSBank, hsram1.Init.ExtendedMode );
    }
    LCD_Display_Dir( 0 );  // 默认为竖屏
    LCD_LED_ON;            // 点亮背光
    LCD_Clear( WHITE );

    /* USER CODE END FMC_Init 2 */
}

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit( void ) {
    /* USER CODE BEGIN FMC_MspInit 0 */

    /* USER CODE END FMC_MspInit 0 */
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if ( FMC_Initialized ) {
        return;
    }
    FMC_Initialized                              = 1;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    /** Initializes the peripherals clock
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
    PeriphClkInitStruct.FmcClockSelection    = RCC_FMCCLKSOURCE_D1HCLK;
    if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK ) {
        Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_ENABLE();

    /** FMC GPIO Configuration
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD13   ------> FMC_A18
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PC7   ------> FMC_NE1
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    */
    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin       = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init( GPIOE, &GPIO_InitStruct );

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init( GPIOD, &GPIO_InitStruct );

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin       = GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_FMC;

    HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    /* USER CODE BEGIN FMC_MspInit 1 */

    /* USER CODE END FMC_MspInit 1 */
}

void HAL_SRAM_MspInit( SRAM_HandleTypeDef* sramHandle ) {
    /* USER CODE BEGIN SRAM_MspInit 0 */

    /* USER CODE END SRAM_MspInit 0 */
    HAL_FMC_MspInit();
    /* USER CODE BEGIN SRAM_MspInit 1 */

    /* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit( void ) {
    /* USER CODE BEGIN FMC_MspDeInit 0 */

    /* USER CODE END FMC_MspDeInit 0 */
    if ( FMC_DeInitialized ) {
        return;
    }
    FMC_DeInitialized = 1;
    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_DISABLE();

    /** FMC GPIO Configuration
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD13   ------> FMC_A18
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PC7   ------> FMC_NE1
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    */

    HAL_GPIO_DeInit( GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 );

    HAL_GPIO_DeInit( GPIOD, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 );

    HAL_GPIO_DeInit( GPIOC, GPIO_PIN_7 );

    /* USER CODE BEGIN FMC_MspDeInit 1 */

    /* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit( SRAM_HandleTypeDef* sramHandle ) {
    /* USER CODE BEGIN SRAM_MspDeInit 0 */

    /* USER CODE END SRAM_MspDeInit 0 */
    HAL_FMC_MspDeInit();
    /* USER CODE BEGIN SRAM_MspDeInit 1 */

    /* USER CODE END SRAM_MspDeInit 1 */
}
/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
