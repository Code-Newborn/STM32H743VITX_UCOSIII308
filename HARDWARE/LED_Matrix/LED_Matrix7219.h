#ifndef __LED_MATRIX7219_H
#define __LED_MATRIX7219_H

#include "stdint.h"

extern uint8_t code_disp1[38][8];

void Max7219_init(void);
void Write_Max7219(uint8_t address, uint8_t data);
void Write_Max7219_byte(uint8_t byte);

#endif //__LED_MATRIX7219_H
