# MPU6050 Driver

## Features
1. **Simple Initialization Position Calculation**: The system includes basic initialization calculations for determining the starting orientation.
2. **DMP Algorithm for Attitude Estimation**: Utilizes the Digital Motion Processor (DMP) within the MPU6050 to estimate attitude, providing Roll, Pitch, and Yaw (RPY) values.
3. **Hardware I2C Communication**: The driver communicates with the MPU6050 sensor using the hardware I2C interface.

## Existing Issues
1. **RPY Drift**: The Roll, Pitch, and Yaw values exhibit drift over time, reducing the accuracy of the attitude estimation.
2. **Attitude Estimation Error**: There are inaccuracies in the estimation of the sensor’s orientation.
3. **MPU6050 Communication Failure After MCU Restart**: After several resets of the microcontroller, the MPU6050 module occasionally pulls the I2C bus low, preventing further communication with the sensor. A full power reset of the MPU6050 is required to restore functionality.

## Planned Improvements
1. **Temperature Compensation**: Implement temperature compensation using the sensor's internal temperature readings to correct drift and improve the accuracy of attitude estimation.
2. **Kalman Filtering**: Integrate a Kalman filter to smooth the attitude estimation and reduce noise, improving overall performance.

 