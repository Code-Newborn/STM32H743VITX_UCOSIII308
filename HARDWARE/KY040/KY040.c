#include "KY040.h"
#include "tim.h"

int TimerEncoder;
int getTimerEncoder( void ) {
    TimerEncoder = ( short )( __HAL_TIM_GET_COUNTER( &htim2 ) );
    __HAL_TIM_SET_COUNTER( &htim2, 0 );
    return TimerEncoder;
}
