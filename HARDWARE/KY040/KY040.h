#ifndef __KY040_H
#define __KY040_H
extern int TimerEncoder;

#include "gpio.h"
#include "tim.h"

int getTimerEncoder( void );

GPIO_PinState GetEncodeStep_Clockwise( void );
GPIO_PinState GetEncodeStep_CounterClockwise( void );

#endif  // __KY040_H
