#include "DigitalServo.h"
#include "math.h"

/**
 * @brief     : 设置舵机角度值
 * @msg       : 依据角度值计算相应脉冲数
 * @param      {float} Angle
 * @return     {*}
 */
void SetServo_Angle(float Angle)
{
    int SetPulse = (int)((Angle / 180.0f) * (Pulses_180_DEG - Pulses_0_DEG) + Pulses_0_DEG);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, SetPulse); // 此时对应180°
}

void SetServo_Angle_WithVel(float Angle, float Vel)
{
    int current = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
    int pulse_deg = (int)((Vel / 180.0f) * (Pulses_180_DEG - Pulses_0_DEG)); // 脉冲数增量
    int SetPulse = (int)((Angle / 180.0f) * (Pulses_180_DEG - Pulses_0_DEG) + Pulses_0_DEG);
    while (abs(SetPulse - current) > pulse_deg)
    {
        if (SetPulse - current >= 0)
        {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, current + pulse_deg); // 此时对应180°
            HAL_Delay((int)(Vel * DEG_Delay));                                 // 根据转动角度大小进行延时
            current = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
        }
        else
        {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, current - pulse_deg); // 此时对应180°
            HAL_Delay((int)(Vel * DEG_Delay));                                 // 根据转动角度大小进行延时
            current = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
        }
    }
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, SetPulse);               // 此时对应180°
    HAL_Delay((int)((SetPulse - current) / pulse_deg * Vel * DEG_Delay)); // 根据转动角度大小进行延时
    current = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
}
