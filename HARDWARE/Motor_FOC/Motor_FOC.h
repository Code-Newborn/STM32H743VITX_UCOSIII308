#ifndef __MOTOR_FOC_H
#define __MOTOR_FOC_H

#include <stdbool.h>
#include <stdint.h>

// 初始变量及函数定义
#define _constrain( amt, low, high ) ( ( amt ) < ( low ) ? ( low ) : ( ( amt ) > ( high ) ? ( high ) : ( amt ) ) )
// 宏定义实现的一个约束函数,用于限制一个值的范围。
// 具体来说，该宏定义的名称为 _constrain，接受三个参数 amt、low 和 high，分别表示要限制的值、最小值和最大值。
// 该宏定义的实现使用了三元运算符，根据 amt 是否小于 low 或大于high，返回其中的最大或最小值，或者返回原值。
// 换句话说，如果 amt 小于 low，则返回 low；如果 amt 大于 high，则返回 high；否则返回amt。
// 这样，_constrain(amt, low, high) 就会将 amt 约束在 [low, high] 的范围内。

void  setTorque( float Uq, float angle_el );
void  FOC_Vbus( float power_supply );
float FOC_M0_Angle( void );
void  Read_Usart( uint8_t* rx, float* arrays );
float _electricalAngle( void );
void  FOC_alignSensor( int _PP, int _DIR );

void Motor_Enable( bool enabale );

#endif  // __MOTOR_FOC_H
