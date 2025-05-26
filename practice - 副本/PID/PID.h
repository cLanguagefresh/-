// PID.h
#ifndef __PID_H_
#define __PID_H_

typedef struct {
    float Kp, Ki, Kd;
    float integral, prev_error;
} PID_Controller;

void PID_Init(void);                  // ��ʼ��PID������
float PID_Update(float error, float dt); // ����PID���
void PID_Run(float target_hum ); // ִ��PID�����߼�

#endif
