// PID.h
#ifndef __PID_H_
#define __PID_H_

typedef struct {
    float Kp, Ki, Kd;
    float integral, prev_error;
} PID_Controller;

void PID_Init(void);                  // 初始化PID控制器
float PID_Update(float error, float dt); // 更新PID输出
void PID_Run(float target, float current, float noise_dB); // 执行PID控制逻辑
// 声明外部变量
extern float control;  // 添加此行

#endif
