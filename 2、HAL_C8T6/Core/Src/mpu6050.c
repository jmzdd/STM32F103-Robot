#include "mpu6050.h"
#include "i2c.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

#define CALIBRATION_SAMPLES 1000

// Helper function to read multiple bytes from a register
static HAL_StatusTypeDef MPU6050_Read_Registers(uint8_t reg, uint8_t *data, uint16_t len) {
    return HAL_I2C_Mem_Read(&hi2c2, MPU6050_ADDR, reg, 1, data, len, HAL_MAX_DELAY);
}

// Helper function to write a single byte to a register
static HAL_StatusTypeDef MPU6050_Write_Register(uint8_t reg, uint8_t data) {
    return HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDR, reg, 1, &data, 1, HAL_MAX_DELAY);
}

void MPU6050_Init(void) {
    uint8_t check;

    // Check device ID WHO_AM_I
    MPU6050_Read_Registers(WHO_AM_I_REG, &check, 1);

    if (check == 104) {  // 0x68 will be returned by the sensor if everything is OK
        // Power management register: Write all 0's to wake the sensor up
        MPU6050_Write_Register(PWR_MGMT_1_REG, 0x01);
        // MPU6050_Write_Register(PWR_MGMT_2_REG, 0x00);
        // MPU6050_Write_Register(SMPLRT_DIV_REG, 0x09);
        // MPU6050_Write_Register(CONFIG_REG, 0x06);
        // MPU6050_Write_Register(GYRO_CONFIG_REG, 0x18);
        // MPU6050_Write_Register(ACCEL_CONFIG_REG, 0x18);
    }
}

void MPU6050_Read_Accel(int16_t* accelData) {
    uint8_t recData[6];

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register
    MPU6050_Read_Registers(ACCEL_XOUT_H, recData, 6);

    accelData[0] = (int16_t)(recData[0] << 8 | recData[1]);
    accelData[1] = (int16_t)(recData[2] << 8 | recData[3]);
    accelData[2] = (int16_t)(recData[4] << 8 | recData[5]);
}

void MPU6050_Read_Gyro(int16_t* gyroData) {
    uint8_t recData[6];

    // Read 6 BYTES of data starting from GYRO_XOUT_H register
    MPU6050_Read_Registers(GYRO_XOUT_H, recData, 6);

    gyroData[0] = (int16_t)(recData[0] << 8 | recData[1]);
    gyroData[1] = (int16_t)(recData[2] << 8 | recData[3]);
    gyroData[2] = (int16_t)(recData[4] << 8 | recData[5]);
}

void MPU6050_GetAccelData(GyroData *gyroDataStruct) {
    int16_t accelData[3];

    // 读取加速度和陀螺仪数据
    MPU6050_Read_Accel(accelData);

    // 创建缓冲区用于存储发送的数据
    char buffer[128];

    sprintf(buffer, "Accel: X=%d, Y=%d, Z=%d\r\n",
            accelData[0], accelData[1], accelData[2]);
    // 将获取到的x,y轴数值传递给结构体变量
    // x值平稳时稳定在[-20,20], y值平稳时稳定在[-80,-100]
    // x轴前大后小, y轴左大右小
    gyroDataStruct->x_value = accelData[0];
    gyroDataStruct->y_value = accelData[1];
    // 使用 HAL_UART_Transmit 发送数据
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}

void MPU6050_GetGyroData(GyroData *gyroDataStruct) {
    int16_t gyroData[3];

    // 读取加速度和陀螺仪数据
    MPU6050_Read_Gyro(gyroData);

    // 创建缓冲区用于存储发送的数据
    char buffer[128];
    sprintf(buffer, "Gyro: X=%d, Y=%d, Z=%d\r\n",
            gyroData[0], gyroData[1], gyroData[2]);
    gyroDataStruct->x_value = gyroData[0];
    gyroDataStruct->y_value = gyroData[1];
    // 使用 HAL_UART_Transmit 发送数据
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}
