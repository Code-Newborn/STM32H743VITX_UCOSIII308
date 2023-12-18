#include "Motor_FOC.h"
#include "math.h"
#include "stdio.h"
#include "AS5600.h"
#include "delay.h"
#include "tim.h"

#define _3PI_2 4.71238898038f
float voltage_power_supply;
float Ualpha, Ubeta = 0, Ua = 0, Ub = 0, Uc = 0;

float zero_electric_angle = 0;
int   PP = 1, DIR = 1;
int   pwmA = 32;
int   pwmB = 33;
int   pwmC = 25;

/**
 * @brief     : 归一化角度到 [0,2PI]
 * @msg       :
 * @param      {float} angle
 * @return     {*}
 */
float _normalizeAngle( float angle ) {
    float a = fmod( angle, 2 * PI );  // 取余运算可以用于归一化，列出特殊值例子一算便知
    return a >= 0 ? a : ( a + 2 * PI );
    // 三目运算符。格式：condition ? expr1 : expr2
    // 其中，condition 是要求值的条件表达式，如果条件成立，则返回 expr1 的值，否则返回 expr2 的值。可以将三目运算符视为 if-else 语句的简化形式。
    // fmod 函数的余数的符号与除数相同。因此，当 angle 的值为负数时，余数的符号将与 _2PI 的符号相反。也就是说，如果 angle 的值小于 0 且 _2PI 的值为正数，则 fmod(angle, _2PI) 的余数将为负数。
    // 例如，当 angle 的值为 -PI/2，_2PI 的值为 2PI 时，fmod(angle, _2PI) 将返回一个负数。在这种情况下，可以通过将负数的余数加上 _2PI 来将角度归一化到 [0, 2PI] 的范围内，以确保角度的值始终为正数。
}

// 设置PWM到控制器输出
void setPwm( float Ua, float Ub, float Uc ) {

    // 计算占空比
    // 限制占空比从0到1
    float dc_a = _constrain( Ua / voltage_power_supply, 0.0f, 1.0f );
    float dc_b = _constrain( Ub / voltage_power_supply, 0.0f, 1.0f );
    float dc_c = _constrain( Uc / voltage_power_supply, 0.0f, 1.0f );

    // TODO 写入PWM到PWM 0 1 2 通道 255为自动重装载值
    __HAL_TIM_SetCompare( &htim1, TIM_CHANNEL_1, dc_a * 255 );
    __HAL_TIM_SetCompare( &htim1, TIM_CHANNEL_2, dc_b * 255 );
    __HAL_TIM_SetCompare( &htim1, TIM_CHANNEL_3, dc_c * 255 );
}

/**
 * @brief     : 设置力矩大小
 * @msg       :
 * @param      {float} Uq
 * @param      {float} angle_el
 * @return     {*}
 */
void setTorque( float Uq, float angle_el ) {
    Uq       = _constrain( Uq, -voltage_power_supply / 2, voltage_power_supply / 2 );  // 电压限幅
    float Ud = 0;
    angle_el = _normalizeAngle( angle_el );  // 电角度

    // 帕克逆变换
    Ualpha = -Uq * sin( angle_el );
    Ubeta  = Uq * cos( angle_el );

    // 克拉克逆变换
    Ua = Ualpha + voltage_power_supply / 2;
    Ub = ( sqrt( 3 ) * Ubeta - Ualpha ) / 2 + voltage_power_supply / 2;
    Uc = ( -Ualpha - sqrt( 3 ) * Ubeta ) / 2 + voltage_power_supply / 2;
    setPwm( Ua, Ub, Uc );  // 设置各相电压
}

/**
 * @brief     : 设置相电压阈值
 * @msg       :
 * @param      {float} power_supply
 * @return     {*}
 */
void DFOC_Vbus( float power_supply ) {
    voltage_power_supply = power_supply;

    // TODO 设置引脚工作模式
    // ledcSetup( 0, 30000, 8 );  // pwm频道, 频率, 精度
    // ledcSetup( 1, 30000, 8 );  // pwm频道, 频率, 精度
    // ledcSetup( 2, 30000, 8 );  // pwm频道, 频率, 精度
    // ledcAttachPin( pwmA, 0 );
    // ledcAttachPin( pwmB, 1 );
    // ledcAttachPin( pwmC, 2 );
    // TODO 串口1打印信息
    printf( "完成PWM初始化设置" );

    // TODO 初始化IIC引脚
    // BeginSensor();
}

/**
 * @brief     : 获取电角度
 * @msg       :
 * @return     {*} 弧度制
 */
float _electricalAngle() {
    return _normalizeAngle( ( float )( DIR * PP ) * AS5600GetAngle() - zero_electric_angle );
}

/**
 * @brief     :
 * @msg       :
 * @param      {int} _PP
 * @param      {int} _DIR
 * @return     {*}
 */
void DFOC_alignSensor( int _PP, int _DIR ) {
    PP  = _PP;
    DIR = _DIR;
    setTorque( 3, _3PI_2 );
    delay( 3000 );
    zero_electric_angle = _electricalAngle();
    setTorque( 0, _3PI_2 );
    printf( "0电角度：" );
    printf( "%.7f", zero_electric_angle );
}

float FOC_M0_Angle() {
    return getAngle();  // 获取弧度角度
}

//==============串口1接收数据==============
// float  motor_target;
// int    commaPosition;
// String serialReceiveUserCommand() {

//     // a string to hold incoming data
//     static String received_chars;

//     String command = "";

//     while ( Serial.available() ) {
//         // get the new byte:
//         char inChar = ( char )Serial.read();
//         // add it to the string buffer:
//         received_chars += inChar;

//         // end of user input
//         if ( inChar == '\n' ) {
//             // execute the user command
//             command = received_chars;

//             commaPosition = command.indexOf( '\n' );  // 检测字符串中的逗号
//             if ( commaPosition != -1 )                // 如果有逗号存在就向下执行
//             {
//                 motor_target = command.substring( 0, commaPosition ).toDouble();  // 电机角度
//                 Serial.println( motor_target );
//             }
//             // reset the command buffer
//             received_chars = "";
//         }
//     }
//     return command;
// }
