#ifndef __AS5600_H
#define __AS5600_H

#include "i2c.h"

#define AS5600_I2C_HANDLE hi2c1

#define I2C_TIME_OUT_BASE 10
#define I2C_TIME_OUT_BYTE 1

#define abs( x ) ( ( x ) > 0 ? ( x ) : -( x ) )  // 求绝对值
#define PI       3.1415926535f

#define AS5600_RAW_ADDR 0x36

/**注意:AS5600的地址0x36是指的是原始7位设备地址,
 * 而ST I2C库中的设备地址是指原始设备地址左移一位得到的设备地址*/
#define AS5600_ADDR       ( AS5600_RAW_ADDR << 1 )
#define AS5600_WRITE_ADDR ( AS5600_RAW_ADDR << 1 )
#define AS5600_READ_ADDR  ( ( AS5600_RAW_ADDR << 1 ) | 1 )

#define AS5600_RESOLUTION 4096  // 12bit Resolution

#define AS5600_RAW_ANGLE_REGISTER 0x0C

extern float angle_raw;
extern float angle;

void     AS5600Init( void );
uint16_t AS5600GetRawAngle( void );
float    AS5600GetAngle( void );

#endif /* __AS5600_H */
