#include "OLED096/SSD1306_Driver.h"
#include "spi.h"
#include <stdint.h>

#define OLED_RESET_LOW()  HAL_GPIO_WritePin( OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET )  // 低电平复位
#define OLED_RESET_HIGH() HAL_GPIO_WritePin( OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET )

#define OLED_CMD_MODE()  HAL_GPIO_WritePin( OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET )  // 命令模式
#define OLED_DATA_MODE() HAL_GPIO_WritePin( OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET )    // 数据模式

#define OLED_CS_HIGH() HAL_GPIO_WritePin( OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET )    // 芯片禁能
#define OLED_CS_LOW()  HAL_GPIO_WritePin( OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET )  // 芯片使能

// 反显函数
void OLED_ColorTurn( uint8_t i ) {
    if ( i == 0 ) {
        WriteCmd( 0xA6 );  // 正常显示
    }
    if ( i == 1 ) {
        WriteCmd( 0xA7 );  // 反色显示
    }
}

// 开启OLED显示
void OLED_DisPlay_On( void ) {
    WriteCmd( 0x8D );  // 电荷泵使能
    WriteCmd( 0x14 );  // 开启电荷泵
    WriteCmd( 0xAF );  // 点亮屏幕
}

// 关闭OLED显示
void OLED_DisPlay_Off( void ) {
    WriteCmd( 0x8D );  // 电荷泵使能
    WriteCmd( 0x10 );  // 关闭电荷泵
    WriteCmd( 0xAF );  // 关闭屏幕
}

void OLED_FlipEnable( void ) {
    WriteCmd( 0xA0 );  // 翻转显示
    WriteCmd( 0xC0 );
}

void OLED_FlipNormal( void ) {
    WriteCmd( 0xA1 );
    WriteCmd( 0xC8 );  // 正常显示
}


void WriteCmd( unsigned char cmd ) {
    OLED_CMD_MODE();
    HAL_SPI_Transmit( &hspi1, &cmd, 1, 200 );  // 通过外设SPIx发送一个数据
}

void WriteDat( unsigned char dat ) {
    OLED_DATA_MODE();
    HAL_SPI_Transmit( &hspi1, &dat, 1, 200 );  // 通过外设SPIx发送一个数据
}

void OLED_Init( void ) {
    OLED_CS_LOW();

    OLED_RESET_LOW();
    HAL_Delay( 200 );
    OLED_RESET_HIGH();  // 复位

    WriteCmd( 0xAE );  // 关闭显示
    WriteCmd( 0x00 );  // 为页面寻址模式设置列起始地址 低位
    WriteCmd( 0x10 );  // 为页面寻址模式设置列起始地址 高位
    WriteCmd( 0x40 );  // 设置显示起始行 (0x40~0x7F)

    WriteCmd( 0x81 );  // 对比度设置
    WriteCmd( 0xCF );  // 对比度值

    WriteCmd( 0xA1 );  // 设置 列与段 重新映射 0xA0左右反置 0xA1正常
    WriteCmd( 0xC8 );  // 设置 COM 输出的扫描方向 0xC0上下反置 0xC8正常

    WriteCmd( 0xA6 );  // 设置 显示方式	0xA6正常显示 0xA7反相显示

    WriteCmd( 0xA8 );  // set multiplex ratio(1 to 64)
    WriteCmd( 0x3f );  // 1/64 duty

    WriteCmd( 0xD3 );  // 显示偏移 (0x00~0x3F)
    WriteCmd( 0x00 );  // not offset

    WriteCmd( 0xd5 );  // set display clock divide ratio/oscillator frequency
    WriteCmd( 0x80 );  // set divide ratio, Set Clock as 100 Frames/Sec

    WriteCmd( 0xD9 );  // 设置预充电时间。
    WriteCmd( 0xF1 );  // 预充电 15 时钟周期 & 放电 1 时钟周期

    WriteCmd( 0xDA );  // 设置 COM 引脚硬件配置
    WriteCmd( 0x12 );

    WriteCmd( 0xDB );  // 调整 VCOMH 稳压器输出
    WriteCmd( 0x40 );  // Set VCOM Deselect Level

    WriteCmd( 0x20 );  // 设置内存寻址模式
    WriteCmd( 0x02 );  // 0x02页寻址 0x01列寻址 0x00行寻址

    WriteCmd( 0x8D );  // set Charge Pump enable/disable
    WriteCmd( 0x14 );  // set(0x10) disable

    WriteCmd( 0xA4 );  // 根据 GDDRAM 内容启用显示输出(0xa4/0xa5)

    WriteCmd( 0xA6 );  // 设置 显示方式	0xA6正常显示 0xA7反相显示

    WriteCmd( 0xAF );  // 开启显示
    OLED_Clear();
}

void OLED_Clear( void )  // 清屏
{
    unsigned char m, n;
    for ( m = 0; m < 8; m++ ) {
        WriteCmd( 0xb0 + m );  // page0-page1
        WriteCmd( 0x00 );      // low column start address
        WriteCmd( 0x10 );      // high column start address
        for ( n = 0; n < 128; n++ ) {
            WriteDat( 0x00 );
        }
    }
}
