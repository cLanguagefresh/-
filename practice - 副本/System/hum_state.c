#include "hum_state.h"
#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "DHT11.h"
#include "PWM.h"
#include "PID.h"
#include "Serial.h"
#include "KEY.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

static HumidifierState g_current_state = HUM_OFF;
static uint8_t g_target_humi = 70;     // Ĭ��Ŀ��ʪ��
static uint32_t g_shutdown_timer = 0;   // �ػ�����ʱ
volatile uint32_t system_time_ms = 0;  // ȷ����SysTick�ж��и���


// SysTick��ʼ����1ms�жϣ�
void SysTick_Init(void) {
    SysTick_Config(SystemCoreClock / 1000);  // 72MHz / 1000 = 72kHz �� 1ms�ж�
}
 //SysTick�жϷ�����
void SysTick_Handler1(void) {
    system_time_ms++;
}

// ��ʼ����ʪ��״̬
void HumState_Init(void) {
    g_current_state = HUM_OFF;
    g_target_humi = 60;
    g_shutdown_timer = 0;
	
	if (g_current_state == HUM_OFF)
		OLED_ShowString(2, 13, "OFF");
	
	if (g_current_state == HUM_MANUAL)
		OLED_ShowString(2, 13, "MAN");
	
	if (g_current_state == HUM_SLEEP)
		OLED_ShowString(2, 13, "SLE");
	
}

// ����״̬������SysTick�жϻ���ѭ���е��ã�
void HumState_Update(void) {
   
    static uint32_t last_update = 0;
    uint32_t now = system_time_ms;
	
	if(g_current_state == HUM_MANUAL || HUM_SLEEP)
		FanAndWater_ON();
		
    
    // �ػ�ģʽ����������ʱ
    if (g_current_state == HUM_OFF) {
		FanAndWater_OFF();
        if (now - g_shutdown_timer >= 10000) { // 10������˯��
            g_current_state = HUM_SLEEP;
             OLED_WriteCommand(0xAE);	//�ر���ʾ
        }
    }
    
    // ˯��ģʽ�¹ر���ʾ����������
    if (g_current_state == HUM_SLEEP) {
        OLED_Clear();
        // ����������������>32dB������PWMռ�ձ�
        if (sound_dB > 65.0f && control > 70.0f) {
        control = 70.0f;
    }

    // ����PWM���
    PWM_SetFanSpeed(control);
    }
}

// ����Ŀ��ʪ��
void HumState_SetTargetHumi(uint8_t humi) {
    if (humi <= 100) {
        g_target_humi = humi;
        OLED_ShowNum(2, 8, (float)humi, 2); // ����������ʾ
    }
	
	
	
	
	
	
}

// ��ȡĿ��ʪ��
uint8_t HumState_GetTargetHumi(void) {
	
	
	
	
    return g_target_humi;
}

// ��ȡ��ǰ״̬
HumidifierState HumState_GetCurrentState(void) {
    return g_current_state; // ֱ�ӷ���״̬���������ֵ
}

// �������¼�
void HumState_HandleKeyEvent(KeyEvent event) {
    switch (event) {
        case KEY_MODE_PRESSED:
            // ѭ���л�״̬��HUM_MANUAL �� HUM_SLEEP �� HUM_OFF
            g_current_state = (g_current_state + 1) % 3;
            if (g_current_state == HUM_OFF) {
                g_shutdown_timer = system_time_ms; // ��ʼ�ػ�����ʱ
				OLED_WriteCommand(0xAF);	//������ʾ
                OLED_ShowString(2, 13, "OFF");
            } else if (g_current_state == HUM_SLEEP) {
				OLED_ShowString(2, 13, "SLE");
                OLED_WriteCommand(0xAE);	//�ر���ʾ
            }
			else {
			OLED_ShowString(2, 13, "MAN");
			OLED_WriteCommand(0xAF);	//������ʾ	
			}
            break;
            
        case KEY_DEC_HUMI_PRESSED:
            if (g_current_state == HUM_MANUAL || g_current_state == HUM_SLEEP) {
                g_target_humi = (g_target_humi >= 5) ? (g_target_humi - 5) : 0;
            }
            break;
            
        case KEY_INC_HUMI_PRESSED:
            if (g_current_state == HUM_MANUAL || g_current_state == HUM_SLEEP) {
                g_target_humi = (g_target_humi <= 95) ? (g_target_humi + 5) : 100;
            }
            break;
            
        default:
            break;
    }
}




