/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		ICM20602
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看common.h内VERSION宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 * @note
					接线定义：
					------------------------------------

					------------------------------------
 ********************************************************************************************************************/
#ifndef __SEEKFREE_ICM20602_H
#define __SEEKFREE_ICM20602_H

#include "main.h"
#include "MahonyAHRS.h"

#define ICM20602_DEV_ADDR 0x69 // SA0接地：0x68   SA0上拉：0x69  模块默认上拉

#define ICM20602_SPI_W 0x00
#define ICM20602_SPI_R 0x80

#define ICM20602_XG_OFFS_TC_H 0x04		// 陀螺仪X轴 高8位 低噪声低功耗偏移移位和陀螺仪偏移温度补偿寄存器
#define ICM20602_XG_OFFS_TC_L 0x05		// 陀螺仪X轴 低8位 低噪声低功耗偏移移位和陀螺仪偏移温度补偿寄存器
#define ICM20602_YG_OFFS_TC_H 0x07		// 陀螺仪Y轴 高8位 低噪声低功耗偏移移位和陀螺仪偏移温度补偿寄存器
#define ICM20602_YG_OFFS_TC_L 0x08		// 陀螺仪Y轴 低8位 低噪声低功耗偏移移位和陀螺仪偏移温度补偿寄存器
#define ICM20602_ZG_OFFS_TC_H 0x0A		// 陀螺仪Z轴 高8位 低噪声低功耗偏移移位和陀螺仪偏移温度补偿寄存器
#define ICM20602_ZG_OFFS_TC_L 0x0B		// 陀螺仪Z轴 低8位 低噪声低功耗偏移移位和陀螺仪偏移温度补偿寄存器
#define ICM_SELF_TESTA_REG 0X10			// 自检寄存器A
#define ICM_MOTION_DET_REG 0X1F			// 运动检测阀值设置寄存器
#define ICM20602_SELF_TEST_X_ACCEL 0x0D // 自检寄存器X
#define ICM20602_SELF_TEST_Y_ACCEL 0x0E // 自检寄存器Y
#define ICM20602_SELF_TEST_Z_ACCEL 0x0F // 自检寄存器Z
#define ICM20602_XG_OFFS_USRH 0x13		// 偏移调整，X轴高8位寄存器
#define ICM20602_XG_OFFS_USRL 0x14		// 偏移调整，X轴低8位寄存器
#define ICM20602_YG_OFFS_USRH 0x15		// 偏移调整，Y轴高8位寄存器
#define ICM20602_YG_OFFS_USRL 0x16		// 偏移调整，Y轴低8位寄存器
#define ICM20602_ZG_OFFS_USRH 0x17		// 偏移调整，Z轴高8位寄存器
#define ICM20602_ZG_OFFS_USRL 0x18		// 偏移调整，Z轴低8位寄存器
#define ICM20602_SMPLRT_DIV 0x19		// 采样频率分频器
#define ICM20602_CONFIG 0x1A			// 配置寄存器
#define ICM20602_GYRO_CONFIG 0x1B		// 陀螺仪 配置寄存器
#define ICM20602_ACCEL_CONFIG 0x1C		// 加速度计 配置寄存器
#define ICM20602_ACCEL_CONFIG_2 0x1D	// 加速度计 低通滤波器设置
#define ICM20602_LP_MODE_CFG 0x1E		// 陀螺仪 低功耗模式配置
#define ICM20602_ACCEL_WOM_X_THR 0x20	// X轴运动中断唤醒的阈值
#define ICM20602_ACCEL_WOM_Y_THR 0x21	// Y轴运动中断唤醒的阈值
#define ICM20602_ACCEL_WOM_Z_THR 0x22	// Z轴运动中断唤醒的阈值
#define ICM20602_FIFO_EN 0x23			// FIFO使能寄存器
#define ICM20602_FSYNC_INT 0x36			// 当产生 FSYNC 中断自动设置为 1。读取寄存器后被设置为 0。
#define ICM20602_INT_PIN_CFG 0x37		// 中断引脚设置
#define ICM20602_INT_ENABLE 0x38
#define ICM20602_FIFO_WM_INT_STATUS 0x39 // FIFO 水印中断状态。读取时清零。
#define ICM20602_INT_STATUS 0x3A		 // 中断状态
#define ICM20602_ACCEL_XOUT_H 0x3B		 // 加速度计测量值 X轴高8位
#define ICM20602_ACCEL_XOUT_L 0x3C		 // 加速度计测量值 X轴低8位
#define ICM20602_ACCEL_YOUT_H 0x3D		 // 加速度计测量值 Y轴高8位
#define ICM20602_ACCEL_YOUT_L 0x3E		 // 加速度计测量值 Y轴低8位
#define ICM20602_ACCEL_ZOUT_H 0x3F		 // 加速度计测量值 Z轴高8位
#define ICM20602_ACCEL_ZOUT_L 0x40		 // 加速度计测量值 Z轴低8位
#define ICM20602_TEMP_OUT_H 0x41		 // 温度测量值 高8位
#define ICM20602_TEMP_OUT_L 0x42		 // 温度测量值 低8位
#define ICM20602_GYRO_XOUT_H 0x43		 // 陀螺仪测量值 X轴高8位
#define ICM20602_GYRO_XOUT_L 0x44		 // 陀螺仪测量值 X轴低8位
#define ICM20602_GYRO_YOUT_H 0x45		 // 陀螺仪测量值 Y轴高8位
#define ICM20602_GYRO_YOUT_L 0x46		 // 陀螺仪测量值 Y轴低8位
#define ICM20602_GYRO_ZOUT_H 0x47		 // 陀螺仪测量值 Z轴高8位
#define ICM20602_GYRO_ZOUT_L 0x48		 // 陀螺仪测量值 Z轴低8位
#define ICM20602_SELF_TEST_X_GYRO 0x50	 // 寄存器中的值表示生产测试期间产生的自检输出
#define ICM20602_SELF_TEST_Y_GYRO 0x51	 // 寄存器中的值表示生产测试期间产生的自检输出
#define ICM20602_SELF_TEST_Z_GYRO 0x52	 // 寄存器中的值表示生产测试期间产生的自检输出
#define ICM20602_FIFO_WM_TH1 0x60		 // 设置 FIFO 水印阈值电平 FIFO_WM_TH[9:0]
#define ICM20602_FIFO_WM_TH2 0x61		 // 设置 FIFO 水印阈值电平 FIFO_WM_TH[9:0]。
#define ICM20602_SIGNAL_PATH_RESET 0x68	 // 重置 加速度计和温度 数字信号路径。
#define ICM20602_ACCEL_INTEL_CTRL 0x69	 // 此位使能 运动唤醒检测逻辑
#define ICM20602_USER_CTRL 0x6A			 // 用户控制
#define ICM20602_PWR_MGMT_1 0x6B		 // 电源管理
#define ICM20602_PWR_MGMT_2 0x6C		 // 传感路径启用/关闭 设置
#define ICM20602_I2C_IF 0x70			 // 禁用 I2C ，将串行接口置于 SPI 模式。
#define ICM20602_FIFO_COUNTH 0x72		 // 表示 FIFO 中的写入字节数 高8位，读取该寄存器时会锁存计数数据
#define ICM20602_FIFO_COUNTL 0x73		 // 表示 FIFO 中的写入字节数 低8位，读取该寄存器时会锁存计数数据
#define ICM20602_FIFO_R_W 0x74			 // 读/写命令 为FIFO提供读或写操作。
#define ICM20602_WHO_AM_I 0x75			 // 指示正在访问哪个设备，默认0x12，ICM-20602 的 I2C 地址为 0x68 或 0x69，具体取决于 AD0 引脚上的驱动值
#define ICM20602_XA_OFFSET_H 0x77		 // 加速度计X轴 偏移消除 高8位。在所有满刻度模式下均可消除 ±16g 偏移。15 位 0.98 mg步进
#define ICM20602_XA_OFFSET_L 0x78		 // 加速度计X轴 偏移消除 低8位。在所有满刻度模式下均可消除 ±16g 偏移。15 位 0.98 mg步进
#define ICM20602_YA_OFFSET_H 0x7A		 // 加速度计Y轴 偏移消除 高8位
#define ICM20602_YA_OFFSET_L 0x7B		 // 加速度计Y轴 偏移消除 低8位
#define ICM20602_ZA_OFFSET_H 0x7D		 // 加速度计Z轴 偏移消除 高8位
#define ICM20602_ZA_OFFSET_L 0x7E		 // 加速度计Z轴 偏移消除 低8位

extern int16_t icm_gyro_x, icm_gyro_y, icm_gyro_z;
extern int16_t icm_acc_x, icm_acc_y, icm_acc_z;

extern float unit_icm_gyro_x;
extern float unit_icm_gyro_y;
extern float unit_icm_gyro_z;
extern float unit_icm_acc_x;
extern float unit_icm_acc_y;
extern float unit_icm_acc_z;

extern float inclination_x;
extern float inclination_y;
extern float inclination_z;

//--------硬件SPI--------------
void icm20602_init_spi(void);
void get_icm20602_accdata_spi(void);
void get_icm20602_gyro_spi(void);
void icm20602_data_change(void);
void icm20602_inclination(void);

#endif /* __SEEKFREE_ICM20602_H */
