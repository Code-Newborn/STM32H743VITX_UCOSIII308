#ifndef __OLED_SSD1306_H
#define __OLED_SSD1306_H

#include "OLED096/SSD1306_Driver.h"

#define OLED_12864
#define SCREEN_PAGE_NUM     ( 8 )
#define SCREEN_PAGEDATA_NUM ( 128 )
#define SCREEN_COLUMN       ( 128 )
#define SCREEN_ROW          ( 64 )


typedef enum {
    pix_black,
    pix_white,
    //............
} Type_color;

//-----------------端口定义----------------

#define OLED_RST_Clr() HAL_GPIO_WritePin( OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET )  // RES
#define OLED_RST_Set() HAL_GPIO_WritePin( OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET )

#define OLED_DC_Clr() HAL_GPIO_WritePin( OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET )  // DC
#define OLED_DC_Set() HAL_GPIO_WritePin( OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET )

#define OLED_CS_Clr() HAL_GPIO_WritePin( OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET )  // CS
#define OLED_CS_Set() HAL_GPIO_WritePin( OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET )

#define OLED_CMD  0  // 写命令
#define OLED_DATA 1  // 写数据

void OLED_WR_BP( uint8_t x, uint8_t y );
void OLED_ShowPicture( uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[] );
void OLED_DrawLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 );

void       SetDrawColor( Type_color value );
Type_color GetDrawColor( void );


void SetPointBuffer( int x, int y, int value );

void loadFlip( void );                 // 屏幕是否翻转
void LCD_FillAll( Type_color color );  // 清屏(指定颜色)
void LCD_Flush( void );                // 屏幕刷新


#endif
