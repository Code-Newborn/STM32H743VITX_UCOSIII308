#ifndef __DIGITALSERVO_H
#define __DIGITALSERVO_H

#include "tim.h"

#define Pulses_0_DEG 650
#define Pulses_180_DEG 2600
#define DEG_Delay 15 // 根据速度确定

#define abs(x) ((x) >= 0 ? (x) : -(x))

void SetServo_Angle(float Angle);
void SetServo_Angle_WithVel(float Angle, float Vel);

#endif // __DIGITALSERVO_H
