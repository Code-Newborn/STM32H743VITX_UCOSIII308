#ifndef __GT911_DRIVER_H
#define __GT911_DRIVER_H

// #include "stm32f10x_exti.h"
#include "ctiic.h"
#include "delay.h"
#include "lcd.h"
#include "sys.h"
#include "touch.h"

#define TOUCH_MAX 5

#define RST_OUT()                            \
    {                                        \
        GPIOI->MODER &= ~( 3 << ( 2 * 8 ) ); \
        GPIOI->MODER |= 1 << 2 * 8;          \
    }  // set RSSET pin to output
#define INT_OUT()                            \
    {                                        \
        GPIOH->MODER &= ~( 3 << ( 2 * 7 ) ); \
        GPIOH->MODER |= 1 << 2 * 7;          \
    }  // set RSSET pin to output
#define INT_IN()                             \
    {                                        \
        GPIOH->MODER &= ~( 3 << ( 2 * 7 ) ); \
        GPIOH->MODER |= 0 << 2 * 7;          \
    }  // set RSSET pin to output
// #define INT_OUT()	{	GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0X00000003;}		//set INT pin to output
// #define INT_IN()	{	GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0X00000004;}		//set INT pin to input

// RST--PI8
// INT--PH7
#define RST_CTRL PIout( 8 )  // GT911 RESET pin out high or low
#define INT_CTRL PHout( 7 )  // GT911 INT pin out high or low
#define INT_GET  PHin( 7 )   // Get GT911 INT pin status

#define GT9XX_IIC_RADDR 0xBB  // IIC read address, should be 0x29
#define GT9XX_IIC_WADDR 0xBA  // IIC write address, should be 0x28

#define GT9XX_READ_ADDR 0x814E  // touch point information
#define GT9XX_ID_ADDR   0x8140  // ID of touch IC

void GT911_int_sync( u16 ms );
void GT911_reset_guitar( u8 addr );
void GT911_gpio_init( void );
u8   GT9XX_WriteHandle( u16 addr );
u8   GT9XX_WriteData( u16 addr, u8 value );
u8   GT9XX_ReadData( u16 addr, u8 cnt, u8* value );
u8   GT911_Init( void );
u8   Touch_Get_Count( void );
u8   GT911_Scan( u8 mode );
void GT9xx_send_config( void );
void GT9xx_Eint_Init( void );

#endif
