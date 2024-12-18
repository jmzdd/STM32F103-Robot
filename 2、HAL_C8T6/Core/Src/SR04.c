#include "SR04.h"
#include <string.h>
#include "stm32f1xx_hal.h"  // 根据具体芯片选择正确的头文件
#include "usart.h"

float distant;      // 测量距离
uint32_t measure_Buf[2] = {0};   // 存放定时器计数值的数组
uint32_t high_time;   // 超声波模块返回的高电平时间

//===============================================读取距离
float SR04_GetData(void)
{
    // 发送Trig信号，启动测距
    HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
    HAL_Delay(1);  // 持续 10-15us 发送脉冲
    HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);

    // 启动定时器输入捕获功能
    __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);  // 启动输入捕获

    // sensing();
    return distant;
}

//===============================================中断回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (TIM2 == htim->Instance)  // 判断触发的中断的定时器为TIM2
    {
        static uint8_t capture_count = 0;

        if (capture_count == 0) {
            // 捕获上升沿时间，记录测量开始的时刻
            measure_Buf[0] = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);  // 设置为下降沿捕获
            capture_count = 1;
        }
        else if (capture_count == 1) {
            // 捕获下降沿时间，记录测量结束的时刻
            measure_Buf[1] = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
            HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);  // 停止输入捕获
            capture_count = 0;

            // 计算高电平时间
            high_time = measure_Buf[1] - measure_Buf[0];  // 高电平时间
            // 发送高电平时间到串口
            char buffer[100];
            // snprintf(buffer, sizeof(buffer), "\r\n----高电平时间-%d-us----\r\n", high_time);
            // HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

            // 计算距离（单位：cm）
            distant = (high_time * 0.034) / 2;
            // snprintf(buffer, sizeof(buffer), "\r\n-检测距离为-%.2f-cm-\r\n", distant);
            // HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

            // 清空定时器计数
            TIM2->CNT = 0;
        }
    }
}
