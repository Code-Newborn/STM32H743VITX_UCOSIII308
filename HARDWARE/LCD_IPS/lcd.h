#ifndef __LCD_H
#define __LCD_H

#include "main.h"
#include "stm32h743xx.h"
#include "sys.h"

#define USE_HORIZONTAL 0  // 设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W 240
#define LCD_H 280

#else
#define LCD_W 280
#define LCD_H 240
#endif

extern uint8_t dma_complete;

//-----------------LCD端口定义----------------

#define LCD_RES_Clr() HAL_GPIO_WritePin( LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET )  // RES
#define LCD_RES_Set() HAL_GPIO_WritePin( LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET )

#define LCD_DC_Clr() HAL_GPIO_WritePin( LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET )  // DC
#define LCD_DC_Set() HAL_GPIO_WritePin( LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET )

#define LCD_CS_Clr() HAL_GPIO_WritePin( LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET )  // CS
#define LCD_CS_Set() HAL_GPIO_WritePin( LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET )

#define LCD_BLK_Clr() HAL_GPIO_WritePin( LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET )  // BLK
#define LCD_BLK_Set() HAL_GPIO_WritePin( LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET )

void LCD_Writ_Bus( uint8_t dat );                                            // 模拟SPI时序
void LCD_WR_DATA8( uint8_t dat );                                            // 写入一个字节
void LCD_WR_DATA16( uint16_t dat );                                          // 写入两个字节
void LCD_WR_REG( uint8_t dat );                                              // 写入一个指令
void LCD_Address_Set( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 );  // 设置坐标函数
void LCD_Init( void );

static void LCD_SPI_Send( uint8_t* data, uint32_t size );   // SPI发送超长字节数据，已经超过 uint16
uint8_t     SPI_WriteData( uint8_t* data, uint16_t size );  // SPI发送多个字节数据，不能超过 uint16

void LCD_Fill( uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color );   // 指定区域填充颜色
void LCD_DrawPoint( uint16_t x, uint16_t y, uint16_t color );                                  // 在指定位置画一个点
void LCD_DrawLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );       // 在指定位置画一条线
void LCD_DrawRectangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );  // 在指定位置画一个矩形
void Draw_Circle( uint16_t x0, uint16_t y0, uint8_t r, uint16_t color );                       // 在指定位置画一个圆

void Show_Str( uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t* str, uint8_t size, uint8_t mode );
void LCD_ShowChinese( uint16_t x, uint16_t y, uint8_t* s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode );       // 显示汉字串
void LCD_ShowChinese12x12( uint16_t x, uint16_t y, uint8_t* s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode );  // 显示单个12x12汉字
void LCD_ShowChinese16x16( uint16_t x, uint16_t y, uint8_t* s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode );  // 显示单个16x16汉字
void LCD_ShowChinese24x24( uint16_t x, uint16_t y, uint8_t* s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode );  // 显示单个24x24汉字
void LCD_ShowChinese32x32( uint16_t x, uint16_t y, uint8_t* s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode );  // 显示单个32x32汉字

void     LCD_ShowChar( uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode );         // 显示一个字符
void     LCD_ShowString( uint16_t x, uint16_t y, const uint8_t* p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode );  // 显示字符串
uint32_t mypow( uint8_t m, uint8_t n );                                                                                      // 求幂
void     LCD_ShowIntNum( uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey );       // 显示整数变量
void     LCD_ShowFloatNum1( uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey );       // 显示两位小数变量

void LCD_ShowPicture( uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[] );  // 显示图片

// 画笔颜色
#define RED       0xF800  // 红色
#define YELLOW    0xFFE0  // 黄色
#define BLUE      0x001F  // 蓝色
#define GREEN     0x07E0  // 绿色
#define CYAN      0X07FF  // 青色
#define MAGENTA   0xF81F  // 紫红色
#define WHITE     0xFFFF  // 白色
#define BLACK     0x0000  // 黑色
#define BROWN     0XBC40  // 棕色
#define BRRED     0XFC07  // 棕红色
#define GRAY      0X8430  // 灰色
#define DARKBLUE  0X01CF  // 暗蓝色
#define LIGHTBLUE 0X7D7C  // 浅蓝色
#define GRAYBLUE  0X5458  // 灰蓝色
#define LGRAY     0XC618  // 浅灰色(PANNEL),窗体背景色

#endif
