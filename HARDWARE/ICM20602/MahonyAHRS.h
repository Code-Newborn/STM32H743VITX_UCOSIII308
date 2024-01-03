#ifndef __MAHONYAHRS_H
#define __MAHONYAHRS_H

//----------------------------------------------------------------------------------------------------
// Variable declaration

extern volatile float Mahony_Kp;         // proportional gain (Kp)
extern volatile float Mahony_Ki;         // integral gain (Ki)
extern volatile float q0, q1, q2, q3;    // quaternion of sensor frame relative to auxiliary frame
extern volatile float roll, pitch, yaw;  // Euler Angle

#define PI      3.1415926f
#define DEG2RAD ( PI / 180.0f )
#define RAD2DEG ( 180.0f / PI )

//---------------------------------------------------------------------------------------------------
// Function declarations

void MahonyAHRSupdate( float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz );
void MahonyAHRSupdateIMU( float gx, float gy, float gz, float ax, float ay, float az );

void Quaternion2Angles( void );

typedef struct QUATERNION {
    float q0;
    float q1;
    float q2;
    float q3;
} MahonyQuaternion;

#endif /* __MAHONYAHRS_H */
