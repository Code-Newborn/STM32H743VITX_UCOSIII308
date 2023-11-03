#ifndef __DELAY_H
#define __DELAY_H
#include "stdint.h"

#define u16 uint16_t
#define u32 uint32_t

void delay_init(u16 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
#endif
