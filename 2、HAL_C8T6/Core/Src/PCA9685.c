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

#include "PCA9685.h"
#include "math.h"
#include "i2c.h"
#include "cmsis_os.h"
#include "mpu6050.h"

uint16_t speed = 150;

uint8_t pca_read(uint8_t startAddress) {
    //Send address to start reading from.
    uint8_t tx[1];
    uint8_t buffer[1];
    tx[0]=startAddress;

    HAL_I2C_Master_Transmit(&hi2c1,pca_adrr, tx,1,10000);
    HAL_I2C_Master_Receive(&hi2c1,pca_adrr,buffer,1,10000);
    return buffer[0];
}

void pca_write(uint8_t startAddress, uint8_t buffer) {
    //Send address to start reading from.
    uint8_t tx[2];
    tx[0]=startAddress;
    tx[1]=buffer;
    HAL_I2C_Master_Transmit(&hi2c1,pca_adrr, tx,2,10000);

}

void pca_setfreq(float freq)//设置PWM频率
{
    uint8_t prescale,oldmode,newmode;
    double prescaleval;
    freq *= 0.92;
    prescaleval = 25000000;
    prescaleval /= 4096;
    prescaleval /= freq;
    prescaleval -= 1;
    prescale =floor(prescaleval + 0.5f);

    oldmode = pca_read(pca_mode1);

    newmode = (oldmode&0x7F) | 0x10; // sleep

    pca_write(pca_mode1, newmode); // go to sleep

    pca_write(pca_pre, prescale); // set the prescaler

    pca_write(pca_mode1, oldmode);
    HAL_Delay(2);

    pca_write(pca_mode1, oldmode | 0xa1);
}

void pca_setpwm(uint8_t num, uint32_t on, uint32_t off)
{
    pca_write(LED0_ON_L+4*num,on);
    pca_write(LED0_ON_H+4*num,on>>8);
    pca_write(LED0_OFF_L+4*num,off);
    pca_write(LED0_OFF_H+4*num,off>>8);
}
/*num:舵机PWM输出引脚0~15，on:PWM上升计数值0~4096,off:PWM下降计数值0~4096
一个PWM周期分成4096份，由0开始+1计数，计到on时跳变为高电平，继续计数到off时
跳变为低电平，直到计满4096重新开始。所以当on不等于0时可作延时,当on等于0时，
off/4096的值就是PWM的占空比。*/

/*
	函数作用：初始化舵机驱动板
	参数：1.PWM频率
		  2.初始化舵机角度
*/
void PCA_MG9XX_Init(float hz)
{
    pca_write(pca_mode1,0x0);
    pca_setfreq(hz);//设置PWM频率
    HAL_Delay(500);
}

/*
	函数作用：控制舵机转动；
	参数：1.输出端口，可选0~15；
		  2.起始角度，可选0~180；
		  3.结束角度，可选0~180；
		  4.模式选择，0 表示函数内无延时，调用时需要在函数后另外加延时函数，且不可调速，第五个参数可填任意值；
					  1 表示函数内有延时，调用时不需要在函数后另外加延时函数，且不可调速，第五个参数可填任意值；
					  2 表示速度可调，第五个参数表示速度值；
		  5.速度，可填大于 0 的任意值，填 1 时速度最快，数值越大，速度越小；
	注意事项：模式 0和1 的速度比模式 2 的最大速度大；
*/
void PCA_MG9XX(uint8_t num,uint8_t end_angle)
{
    uint32_t off=0;
    off=(uint32_t)(80+end_angle*2.2);
    pca_setpwm(num,0,off);
}

void SetAngle(uint8_t channel,uint8_t angle)
{
    uint32_t off=0;
    off=(uint32_t)(102.4*(1+angle/45));
    pca_setpwm(channel,0,off);
}

/* 功能函数 */
void advance_bot_init(void) {
    SetAngle(1,0);
    SetAngle(2,90);

    SetAngle(5,180);
    SetAngle(6,90);

    SetAngle(9,0);
    SetAngle(10,45);

    SetAngle(13,180);
    SetAngle(14,145);
}

void back_bot_init(void) {
    SetAngle(13,180);
    SetAngle(14,90);

    SetAngle(9,0);
    SetAngle(10,90);

    SetAngle(5,180);
    SetAngle(6,45);

    SetAngle(1,0);
    SetAngle(2,145);
}


void advance(void) {
  // 姿态初始化
  // advance_bot_init();
  // 第一组抬腿
  // SetAngle(1,45);
  // SetAngle(13,135);
  // osDelay(speed);
  // 第一组伸腿
  SetAngle(2,135);
  SetAngle(14,90);
  osDelay(speed);
  // 第一组下落同时开始回收
  SetAngle(1,0);
  SetAngle(13,180);
  osDelay(speed-30);
  // 第一组回收
  SetAngle(2,90);
  SetAngle(14,135);
  // 至此第一部分已经结束
  // 同时另一组开始抬腿
  SetAngle(5,135);
  SetAngle(9,45);
  osDelay(speed);
  // 第二组伸腿
  SetAngle(10,90);
  SetAngle(6,45);
  osDelay(speed);
  // 第二组下落同时开始回收
  SetAngle(5,180);
  SetAngle(9,0);
  osDelay(speed-30);
  // 第二组回收
  SetAngle(6,90);
  SetAngle(10,45);
  // 至此第二组已经结束
  // 同时第一组开始抬腿...
    SetAngle(1,45);
    SetAngle(13,135);
}

void back(void) {
  // 姿态初始化
  // back_bot_init();
  // 第一组抬腿
  // SetAngle(13,135);
  // SetAngle(1,45);
  // osDelay(speed);
  // 第一组伸腿
  SetAngle(14,135);
  SetAngle(2,90);
  osDelay(speed);
  // 第一组下落同时开始回收
  SetAngle(13,180);
  SetAngle(1,0);
  osDelay(speed-30);
  // 第一组回收
  SetAngle(14,90);
  SetAngle(2,145);
  // 至此第一部分已经结束
  // 同时另一组开始抬腿
  SetAngle(9,45);
  SetAngle(5,135);
  osDelay(speed);
  // 第二组伸腿
  SetAngle(10,45);
  SetAngle(6,90);
  osDelay(speed);
  // 第二组下落同时开始回收
  SetAngle(9,0);
  SetAngle(5,180);
  osDelay(speed-30);
  // 第二组回收
  SetAngle(10,90);
  SetAngle(6,45);
  // 至此第二组已经结束
  // 同时第一组开始抬腿...
  SetAngle(13,135);
  SetAngle(1,45);

}

void turn_left(void) {
  advance_bot_init();
  // 第一组抬腿
  SetAngle(1,45);
  SetAngle(13,135);
  osDelay(speed);
  // 第一组挪腿
  SetAngle(2,90);
  SetAngle(14,90);
  osDelay(speed);
  // 第一组下落同时准备开始回收
  SetAngle(1,0);
  SetAngle(13,180);
  // 至此第一部分已经结束
  // 同时另一组开始抬腿
  SetAngle(5,135);
  SetAngle(9,45);
  osDelay(speed);
  // 第二组挪腿
  SetAngle(6,45);
  SetAngle(10,0);
  osDelay(speed);
  // 第二组下落
  SetAngle(5,180);
  SetAngle(9,0);
  osDelay(speed);
  // 四足归位
  SetAngle(2,90);
  SetAngle(14,135);
  SetAngle(6,90);
  SetAngle(10,45);
}

void turn_right(void) {
  advance_bot_init();
  // 第一组抬腿
  SetAngle(5,135);
  SetAngle(9,45);
  osDelay(speed);
  // 第一组挪腿
  SetAngle(6,90);
  SetAngle(10,90);
  osDelay(speed);
  // 第一组下落同时准备开始回收
  SetAngle(5,180);
  SetAngle(9,0);
  // 至此第一部分已经结束
  // 同时另一组开始抬腿
  SetAngle(1,45);
  SetAngle(13,135);
  osDelay(speed);
  // 第二组挪腿
  SetAngle(2,135);
  SetAngle(14,180);
  osDelay(speed);
  // 第二组下落
  SetAngle(1,0);
  SetAngle(13,180);
  osDelay(speed);
  // 四足归位
  SetAngle(6,90);
  SetAngle(10,45);
  SetAngle(2,90);
  SetAngle(14,135);
}