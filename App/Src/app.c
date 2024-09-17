#include "app.h"
#include "mpu6050.h"
#include <math.h>

float pitch, roll, yaw;         // DMP中计算的角度
float pitch_init, roll_init;    // 初始水平角度
int16_t ax, ay, az, gx, gy, gz; // 原始值

void App()
{
    // 初始化
    while (MPU6050_Init())
    {
        LOG_INFO("MPU6050 ID ERROR");
    }

    while (1)
    {
        uint8_t res = mpu_dmp_init();
        if (res == 0)
            break;
        LOG_INFO("mpu_dmp_init failed %d", res);
        HAL_Delay(20);
    }

    // 找出水平初始化的角度
    int sample_time = 100;
    float accel_x = 0, accel_y = 0, accel_z = 0;
    for (int i = 0; i < sample_time; i++)
    {
        MPU6050_Get_Accelerometer(&ax, &ay, &az);
        accel_x = accel_x + ax / 16384.0;
        accel_y = accel_y + ay / 16384.0;
        accel_z = accel_z + az / 16384.0;
    }
    accel_x /= 100, accel_y /= 100, accel_z /= 100;
    // 计算Pitch和Roll角度
    roll_init = atan2(accel_y, accel_z) * 57.2957795f;
    pitch_init = atan2(-accel_x, sqrt(accel_y * accel_y + accel_z * accel_z)) * 57.2957795f;

    // 显示
    while (1)
    {
        MPU6050_Get_Gyroscope(&gx, &gy, &gz);
        MPU6050_Get_Accelerometer(&ax, &ay, &az);
        mpu_dmp_get_data(&pitch, &roll, &yaw);
        LOG_INFO_T(1, "Gyroscope: %f %f %f", gx / 16.4, gy / 16.4, gz / 16.4);
        LOG_INFO_T(2, "Accelerometer: %f %f %f", ax / 16384.0, ay / 16384.0, az / 16384.0);
        LOG_INFO_T(3, "PRY: %f %f %f Init: %f %f  result: %f %f", roll, pitch, yaw, roll_init, pitch_init, roll + roll_init, pitch + pitch_init);
        HAL_Delay(2);
    }
}
