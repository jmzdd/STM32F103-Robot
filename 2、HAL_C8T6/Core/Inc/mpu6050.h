#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f1xx_hal.h"

// MPU6050 register addresses
#define MPU6050_ADDR 0xD0
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define PWR_MGMT_2_REG 0x6C
#define SMPLRT_DIV_REG 0x19
#define CONFIG_REG 0x1A
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

typedef struct {
    int x_value;
    int y_value;
} GyroData;

// Function prototypes
void MPU6050_Init(void);
void MPU6050_Read_Accel(int16_t* accelData);
void MPU6050_Read_Gyro(int16_t* gyroData);
void MPU6050_GetAccelData(GyroData *gyroDataStruct);
void MPU6050_GetGyroData(GyroData *gyroDataStruct);

#endif /* MPU6050_H */
