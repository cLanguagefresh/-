// PID.c
#include "stm32f10x.h"
#include "PID.h"
#include "PWM.h"
#include "OLED.h"
#include "AD.h"
#include "hum_state.h"

static PID_Controller pid;  // ��̬ȫ�ֱ�����ȷ��״̬����

float error = 0;
float control = 0;  // �����������1��

// ��ʼ��PID����
void PID_Init(void) {
    pid.Kp = 2.0;
    pid.Ki = 0.1;
    pid.Kd = 0.05;
    pid.integral = 0;
    pid.prev_error = 0;
}

// ����PID������贫������ʱ������
float PID_Update(float error, float dt) {
    float derivative = (error - pid.prev_error) / dt;
    float output_unclamped = pid.Kp * error + pid.Ki * pid.integral + pid.Kd * derivative;

    // ����޷���0%~100%��
    float output = output_unclamped;
    if (output < 0) output = 0;
    if (output > 100) output = 100;

    // �����ֱ��ͣ��������δ�޷�ʱ���»���
    if (output == output_unclamped) {
        pid.integral += error * dt;
    } else {
        pid.integral -= error * dt;  // ������Ч����
    }

    pid.prev_error = error;
    return output;
}

// ִ��������PID�����߼���Ŀ��ֵ����ǰֵ��������
void PID_Run(float target, float current, float noise_dB) {
     error = target - current;
     control = PID_Update(error, 1.0f);  // �����������1��

    // �������ƣ�������>65dB���������ռ�ձ�Ϊ70%
    if (noise_dB > 65.0f && control > 70.0f) {
        control = 70.0f;
    }

    // ����PWM���
    PWM_SetFanSpeed(control);

    
    OLED_ShowString(1, 11, "P:");
    OLED_ShowNum(1, 13, (int)control, 3);
}
