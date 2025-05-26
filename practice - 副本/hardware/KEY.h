#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

// �������Ŷ���
#define MODE_KEY_PIN    GPIO_Pin_15   // PA15: ģʽ��
#define DEC_HUMI_PIN    GPIO_Pin_12   // PA12: ʪ��-
#define INC_HUMI_PIN    GPIO_Pin_11   // PA11: ʪ��+

// �����¼�����
 typedef enum {
    KEY_EVENT_NONE,
    KEY_MODE_PRESSED,     // ģʽ������
    KEY_DEC_HUMI_PRESSED, // ʪ��-����
    KEY_INC_HUMI_PRESSED  // ʪ��+����
} KeyEvent;

// ��������
void Key_Init(void);
KeyEvent Key_Scan(void);
void FanAndWater_ON(void);
void FanAndWater_OFF(void);

#endif /* __KEY_H */
