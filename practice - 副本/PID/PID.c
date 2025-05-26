// PID.c
#include "stm32f10x.h"
#include "PID.h"
#include "PWM.h"
#include "OLED.h"
#include "AD.h"
#include "Serial.h"
#include "DHT11.h"
#include "hum_state.h"

static PID_Controller pid;  // ��̬ȫ�ֱ�����ȷ��״̬����
static uint32_t last_update_time = 0;
 
float control =0.0f ;


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

    // �Ż����ֿ������߼�
    if (output == output_unclamped) {
        pid.integral += error * dt;
    } else {
        // �����޷�������˻���
        if (output_unclamped > 100) {
            float excess = output_unclamped - 100;
            pid.integral -= (excess / pid.Ki); // ����������
        } else if (output_unclamped < 0) {
            float deficit = 0 - output_unclamped;
            pid.integral += (deficit / pid.Ki);
        }
    }

    pid.prev_error = error;
    return output;
}


// ִ��������PID�����߼���Ŀ��ֵ��
void PID_Run(float target_hum ) {
	
	float dt = (system_time_ms - last_update_time) / 1000.0f;
    last_update_time = system_time_ms;

    float error = target_hum - humidity;
    float control = PID_Update(error, dt);  // ʹ�ö�̬ʱ����
   
	if (sound_dB > 65.0f && control > 70.0f) {
        control = 70.0f;
    }

    // ����PWM���
    PWM_SetFanSpeed(control);

    // ��ʾʵʱ����
  //  OLED_ShowString(2, 1, "Hum:");
  //  OLED_ShowNum(2, 6, (int)current, 2);
    OLED_ShowString(1, 11, "PWM:");
    OLED_ShowNum(2, 11, (int)control, 3);
}
