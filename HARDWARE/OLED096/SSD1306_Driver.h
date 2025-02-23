#ifndef __SSD1306_DRIVER_H
#define __SSD1306_DRIVER_H

#include <stdint.h>
#include <stdlib.h>
#include "delay/delay.h"
#include "spi.h"

void WriteDat( unsigned char dat );
void WriteCmd( unsigned char cmd );

void OLED_Init( void );
void OLED_Clear( void );
void OLED_FlipNormal( void );
void OLED_FlipEnable( void );
void OLED_DisPlay_Off( void );
void OLED_DisPlay_On( void );
void OLED_DisplayTurn( uint8_t i );
void OLED_ColorTurn( uint8_t i );

#endif
