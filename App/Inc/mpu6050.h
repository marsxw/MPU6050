#ifndef MPU6050_H
#define MPU6050_H
#include "i2c.h"
#include "stm32f4xx.h"
#include "log.h"
#include <stdbool.h>

#define MPU6050_ADDR 0x68        // MPU6050 I2C地址
#define MPU6050_POWER_ADDR 0x6B  // MPU6050 电源地址
#define MPU6050_POWER2_ADDR 0x6C // MPU6050 电源2地址
#define MPU_GYRO_CFG 0x1B        // 陀螺仪配置寄存器地址
#define MPU_ACCEL_CFG 0x1C       // 加速度传感器配置寄存器地址
#define MPU_INT_CFG 0x37         // 中断INT引脚有效
#define MPU_INT_ENABLE 0x38      // 中断使能寄存器地址
#define MPU_USER_CTRL 0x6A       // 用户控制寄存器地址
#define MPU_SAMPLE_RATE_DIV 0x19 // 采样率分频寄存器地址
#define MPU_CONFIG 0x1A          // 配置寄存器地址
#define MPU_FIFO_EN 0x23         // FIFO 使能寄存器地址
#define MPU_DEVICE_ID_REG 0x75   // 设备ID寄存器地址

#define MPU_TEMPERATRUE_HIGH 0X41 // 温度寄存器的高低地址
#define MPU_TEMPERATRUE_LOW 0X42
#define MPU_GYROSCOPE 0X43     // 陀螺仪数据地址
#define MPU_ACCELEROMETER 0X3B // 加速度数据地址

#define MPU6050_IIC hi2c1

extern uint8_t mpu6050_write_pbuf[8];
extern uint8_t mpu6050_read_pbuf[8];

bool MPU6050_Write_Bytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
bool MPU6050_Read_Bytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
bool MPU6050_Get_Accelerometer(int16_t *ax, int16_t *ay, int16_t *az);
bool MPU6050_Get_Gyroscope(int16_t *gx, int16_t *gy, int16_t *gz);
float MPU6050_Get_Temperature();
uint8_t MPU6050_Init();
#endif
