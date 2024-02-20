#include "KY040.h"

int TimerEncoder;
int getTimerEncoder( void ) {
    TimerEncoder = ( short )( __HAL_TIM_GET_COUNTER( &htim2 ) );
    __HAL_TIM_SET_COUNTER( &htim2, 0 );
    return TimerEncoder;
}

// 顺时针触发
GPIO_PinState GetEncodeStep_Clockwise( void ) {
    TimerEncoder = ( short )( __HAL_TIM_GET_COUNTER( &htim2 ) );
    if ( TimerEncoder >= 1 ) {
        return GPIO_PIN_RESET;
    } else {
        return GPIO_PIN_SET;
    }
}

// 逆时针触发
GPIO_PinState GetEncodeStep_CounterClockwise( void ) {
    TimerEncoder = ( short )( __HAL_TIM_GET_COUNTER( &htim2 ) );
    if ( TimerEncoder <= -1 ) {
        return GPIO_PIN_RESET;
    } else {
        return GPIO_PIN_SET;
    }
}
