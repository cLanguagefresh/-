#ifndef __HUM_STATE_H
#define __HUM_STATE_H

#include "KEY.h"  // ���� Key.h �Ի�ȡ KeyEvent ���Ͷ���

// ��ʪ��״̬ö��
typedef enum {
    HUM_MANUAL,  // �ֶ�ģʽ
    HUM_SLEEP,   // ˯��ģʽ
    HUM_OFF      // �ػ�ģʽ
} HumidifierState;

// ״̬������
void HumState_Init(void);
void HumState_Update(void);
void HumState_SetTargetHumi(uint8_t humi);
uint8_t HumState_GetTargetHumi(void);
HumidifierState HumState_GetCurrentState(void);

void SysTick_Init(void);
void SysTick_Handler(void);

// �������¼�
void HumState_HandleKeyEvent(KeyEvent event);
void parseJSON(const char* json, const char* key, int* value);
void HumState_HandleRemoteCommand(const char* command) ;

#endif /* __HUM_STATE_H */
