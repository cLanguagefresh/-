// PID.c
#include "stm32f10x.h"
#include "PID.h"
#include "PWM.h"
#include "OLED.h"
#include "AD.h"
#include "hum_state.h"

static PID_Controller pid;  // 静态全局变量，确保状态持续

float error = 0;
float control = 0;  // 假设采样周期1秒

// 初始化PID参数
void PID_Init(void) {
    pid.Kp = 2.0;
    pid.Ki = 0.1;
    pid.Kd = 0.05;
    pid.integral = 0;
    pid.prev_error = 0;
}

// 更新PID输出（需传入误差和时间间隔）
float PID_Update(float error, float dt) {
    float derivative = (error - pid.prev_error) / dt;
    float output_unclamped = pid.Kp * error + pid.Ki * pid.integral + pid.Kd * derivative;

    // 输出限幅（0%~100%）
    float output = output_unclamped;
    if (output < 0) output = 0;
    if (output > 100) output = 100;

    // 抗积分饱和：仅在输出未限幅时更新积分
    if (output == output_unclamped) {
        pid.integral += error * dt;
    } else {
        pid.integral -= error * dt;  // 回退无效积分
    }

    pid.prev_error = error;
    return output;
}

// 执行完整的PID控制逻辑（目标值、当前值、噪声）
void PID_Run(float target, float current, float noise_dB) {
     error = target - current;
     control = PID_Update(error, 1.0f);  // 假设采样周期1秒

    // 噪声抑制：若噪声>65dB，限制最大占空比为70%
    if (noise_dB > 65.0f && control > 70.0f) {
        control = 70.0f;
    }

    // 更新PWM输出
    PWM_SetFanSpeed(control);

    
    OLED_ShowString(1, 11, "P:");
    OLED_ShowNum(1, 13, (int)control, 3);
}
