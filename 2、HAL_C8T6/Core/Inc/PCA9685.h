/***************************************************
This is a library for our Adafruit 16-channel PWM & Servo driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef __STM32PCA9685_H
#define __STM32PCA9685_H

//#include "stm32f10x.h"
#include "stm32f1xx_hal.h"

#define pca_adrr 0x80

#define pca_mode1 0x0
#define pca_pre 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define jdMIN  115 // minimum
#define jdMAX  590 // maximum
#define jd000  130 //0度对应4096的脉宽计数值
#define jd180  520 //180度对应4096的脉宽计算值


void pca_write(uint8_t adrr,uint8_t data);
uint8_t pca_read(uint8_t adrr);
void PCA_MG9XX_Init(float hz);
void pca_setfreq(float freq);
void pca_setpwm(uint8_t num, uint32_t on, uint32_t off);
void PCA_MG9XX(uint8_t num,uint8_t end_angle);
void SetAngle(uint8_t channel,uint8_t angle);

/* 功能函数 */
void advance_bot_init(void);
void back_bot_init(void);
void advance(void);
void back(void);
void turn_left(void);
void turn_right(void);

#endif









