#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include <stdlib.h>
#include "main.h"
#include "sys.h"

//-----------------测试LED端口定义----------------

#define LED_ON  HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET )
#define LED_OFF HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_SET )

//-----------------OLED端口定义----------------

// #define OLED_SCL_Clr() GPIO_ResetBits( GPIOG, GPIO_Pin_12 )  // SCL
// #define OLED_SCL_Set() GPIO_SetBits( GPIOG, GPIO_Pin_12 )

// #define OLED_SDA_Clr() GPIO_ResetBits( GPIOD, GPIO_Pin_5 )  // SDA
// #define OLED_SDA_Set() GPIO_SetBits( GPIOD, GPIO_Pin_5 )

#define OLED_RES_Clr() HAL_GPIO_WritePin( INK_RES_GPIO_Port, INK_RES_Pin, GPIO_PIN_RESET )  // RES
#define OLED_RES_Set() HAL_GPIO_WritePin( INK_RES_GPIO_Port, INK_RES_Pin, GPIO_PIN_SET )

#define OLED_DC_Clr() HAL_GPIO_WritePin( INK_DC_GPIO_Port, INK_DC_Pin, GPIO_PIN_RESET )  // DC
#define OLED_DC_Set() HAL_GPIO_WritePin( INK_DC_GPIO_Port, INK_DC_Pin, GPIO_PIN_SET )

#define OLED_CS_Clr() HAL_GPIO_WritePin( INK_CS_GPIO_Port, INK_CS_Pin, GPIO_PIN_RESET )  // CS
#define OLED_CS_Set() HAL_GPIO_WritePin( INK_CS_GPIO_Port, INK_CS_Pin, GPIO_PIN_SET )

#define OLED_BUSY() HAL_GPIO_ReadPin( INK_BUSY_GPIO_Port, INK_BUSY_Pin )

#define OLED_W 128
#define OLED_H 296

typedef struct {
    uint8_t* Image;
    uint16_t Width;
    uint16_t Height;
    uint16_t WidthMemory;
    uint16_t HeightMemory;
    uint16_t Color;
    uint16_t Rotate;
    uint16_t WidthByte;
    uint16_t HeightByte;
} PAINT;
extern PAINT Paint;

#define ROTATE_0   0    // 屏幕正向显示
#define ROTATE_90  90   // 屏幕旋转90度显示
#define ROTATE_180 180  // 屏幕旋转180度显示
#define ROTATE_270 270  // 屏幕旋转270度显示

#define WHITE 0xFF  // 显示白色
#define BLACK 0x00  // 显示黑色

// void OLED_GPIOInit( void );                                                  // 初始化GPIO
void OLED_WR_Bus( uint8_t dat );                                             // 模拟SPI时序
void OLED_WR_REG( uint8_t reg );                                             // 写入一个命令
void OLED_WR_DATA8( uint8_t dat );                                           // 写入一个字节
void OLED_AddressSet( uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye );  // 设置位置函数
void OLED_Init( void );                                                      // 初始化屏幕

void Epaper_READBUSY( void );
void EPD_Update( void );

void Paint_NewImage( uint8_t* image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color );                  // 创建画布控制显示方向
void OLED_Clear( uint16_t Color );                                                                                        // 清屏函数
void OLED_DrawPoint( uint16_t Xpoint, uint16_t Ypoint, uint16_t Color );                                                  // 画点
void OLED_DrawLine( uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color );                     // 画线
void OLED_DrawRectangle( uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color, uint8_t mode );  // 画矩形
void OLED_DrawCircle( uint16_t X_Center, uint16_t Y_Center, uint16_t Radius, uint16_t Color, uint8_t mode );              // 画圆
void OLED_ShowChar( uint16_t x, uint16_t y, uint16_t chr, uint16_t size1, uint16_t color );                               // 显示字符
void OLED_ShowString( uint16_t x, uint16_t y, uint8_t* chr, uint16_t size1, uint16_t color );                             // 显示字符串
void OLED_ShowNum( uint16_t x, uint16_t y, uint32_t num, uint16_t len, uint16_t size1, uint16_t color );                  // 显示数字
void OLED_ShowChinese( uint16_t x, uint16_t y, uint16_t num, uint16_t size1, uint16_t color );                            // 显示中文
void OLED_ShowPicture( uint16_t x, uint16_t y, uint16_t sizex, uint16_t sizey, const uint8_t BMP[], uint16_t color );     // 显示图片
void OLED_Display( unsigned char* Image );                                                                                // 更新到屏幕
void OLED_GUIInit( void );                                                                                                // 屏幕GUI初始化

#endif
