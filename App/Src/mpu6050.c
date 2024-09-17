#include "mpu6050.h"
uint8_t mpu6050_write_pbuf[8];
uint8_t mpu6050_read_pbuf[8];

/**
 * @brief 写入 MPU6050 寄存器
 */
bool MPU6050_Write_Bytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    return HAL_I2C_Mem_Write(&MPU6050_IIC, addr << 1 | 0, reg, I2C_MEMADD_SIZE_8BIT, buf, len, HAL_MAX_DELAY);
}

/**
 * @brief 读取 MPU6050 寄存器
 */
bool MPU6050_Read_Bytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    return HAL_I2C_Mem_Read(&MPU6050_IIC, addr << 1 | 1, reg, I2C_MEMADD_SIZE_8BIT, buf, len, HAL_MAX_DELAY);
}

bool MPU6050_Get_Accelerometer(int16_t *ax, int16_t *ay, int16_t *az)
{
    memset(mpu6050_read_pbuf, 0, sizeof(mpu6050_read_pbuf));
    bool res = MPU6050_Read_Bytes(MPU6050_ADDR, MPU_ACCELEROMETER, 6, mpu6050_read_pbuf);
    *ax = ((uint16_t)mpu6050_read_pbuf[0] << 8) | mpu6050_read_pbuf[1];
    *ay = ((uint16_t)mpu6050_read_pbuf[2] << 8) | mpu6050_read_pbuf[3];
    *az = ((uint16_t)mpu6050_read_pbuf[4] << 8) | mpu6050_read_pbuf[5];
    return res;
}

bool MPU6050_Get_Gyroscope(int16_t *gx, int16_t *gy, int16_t *gz)
{
    memset(mpu6050_read_pbuf, 0, sizeof(mpu6050_read_pbuf));
    bool res = MPU6050_Read_Bytes(MPU6050_ADDR, MPU_GYROSCOPE, 6, mpu6050_read_pbuf);
    *gx = ((uint16_t)mpu6050_read_pbuf[0] << 8) | mpu6050_read_pbuf[1];
    *gy = ((uint16_t)mpu6050_read_pbuf[2] << 8) | mpu6050_read_pbuf[3];
    *gz = ((uint16_t)mpu6050_read_pbuf[4] << 8) | mpu6050_read_pbuf[5];
    return res;
}

/**
 * @brief 读取温度
 *
 * @return float
 */
float MPU6050_Get_Temperature()
{
    memset(mpu6050_read_pbuf, 0, sizeof(mpu6050_read_pbuf));
    MPU6050_Read_Bytes(MPU6050_ADDR, MPU_TEMPERATRUE_HIGH, 6, mpu6050_read_pbuf);
    return ((int16_t)((mpu6050_read_pbuf[0] << 8) | mpu6050_read_pbuf[1])) / 340.0 + 36.53;
}

/**
 * @brief 初始化 MPU6050
 */
uint8_t MPU6050_Init()
{

    // 第一步：复位 MPU6050
    mpu6050_write_pbuf[0] = 0x80; // bit 7 置 1 复位 MPU6050
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU6050_POWER_ADDR, 1, mpu6050_write_pbuf);
    HAL_Delay(100);                     // 给设备一点时间完成复位
    mpu6050_write_pbuf[0] = 0b00000001; // 唤醒 MPU6050
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU6050_POWER_ADDR, 1, mpu6050_write_pbuf);
    HAL_Delay(50);
    // 读取模块ID
    MPU6050_Read_Bytes(MPU6050_ADDR, MPU_DEVICE_ID_REG, 1, mpu6050_read_pbuf);
    LOG_DEBUG("MPU6050 ID: %x READ ID: %x", MPU6050_ADDR, mpu6050_read_pbuf[0]);
    if (mpu6050_read_pbuf[0] != MPU6050_ADDR)
    {
        return 1; // 读取失败
    }
    // 使能角速度和加速度传感器
    mpu6050_write_pbuf[0] = 0x00;                                                  // 使能加速度和陀螺仪（将电源管理寄存器2的相关位设置为0）
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU6050_POWER2_ADDR, 1, mpu6050_write_pbuf); // 电源管理寄存器2 (0x6C)

    // 第二步：设置陀螺仪满量程范围 ±2000dps
    mpu6050_write_pbuf[0] = 3 << 3;
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU_GYRO_CFG, 1, mpu6050_write_pbuf);

    // 第三步：设置加速度传感器满量程范围为 ±2g
    mpu6050_write_pbuf[0] = 0 << 3;
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU_ACCEL_CFG, 1, mpu6050_write_pbuf);

    // 第四步：关闭中断
    mpu6050_write_pbuf[0] = 0x80;                                          // INT引脚低电平有效，配置中断配置寄存器
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU_INT_CFG, 1, mpu6050_write_pbuf); // 中断配置寄存器：0x37
    mpu6050_write_pbuf[0] = 0x00;
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU_INT_ENABLE, 1, mpu6050_write_pbuf);

    // 第五步：关闭 AUX I2C 接口
    mpu6050_write_pbuf[0] = 0x00;
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU_USER_CTRL, 1, mpu6050_write_pbuf);

    // 第六步：设置采样率为 50Hz
    mpu6050_write_pbuf[0] = 1000 / 50 - 1; // 采样率=1000Hz / (1 + 分频值)
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU_SAMPLE_RATE_DIV, 1, mpu6050_write_pbuf);

    // 第七步：设置数字低通滤波器 (DLPF) 带宽为 42Hz
    mpu6050_write_pbuf[0] = 0x03;
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU_CONFIG, 1, mpu6050_write_pbuf);

    // 第八步：关闭 FIFO
    mpu6050_write_pbuf[0] = 0x00; // 禁止所有 FIFO 通道
    MPU6050_Write_Bytes(MPU6050_ADDR, MPU_FIFO_EN, 1, mpu6050_write_pbuf);

    return 0;
}
