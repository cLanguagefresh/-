// main.c
#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "DHT11.h"
#include "PID.h"
#include "Serial.h"
#include "hum_state.h"
#include "RemoteCommand.h"

int main(void) {
    OLED_Init();
    AD_Init();
    DHT11_GPIO_Config();
    PID_Init();           // ��ʼ��PID������
    SysTick_Init(); 	// ��ʼ��SysTick����main.c��ʵ�֣�
	Serial_Init();
	Key_Init();  // ��ʼ������ģ��
	HumState_Init();
	
    while (1) {
		
		// 1. ɨ�谴���������¼�
        KeyEvent event = Key_Scan();
        if (event != KEY_EVENT_NONE) {
            HumState_HandleKeyEvent(event);
        }
		// 3. ���¼�ʪ��״̬
        HumState_Update();
		
        Adc();            // ����������ˮλ����
        Dht11();          // ������ʪ������
		Send_Sensor_Data();
		RemoteCommand_Process(); // ���������
        // ִ��PID���ƣ�Ŀ��ʪ��85%RH����ǰʪ��humidity������sound_dB��
		float target_hum = (float)HumState_GetTargetHumi();
		OLED_ShowNum(2, 8, target_hum, 2);
		PID_Run( target_hum,  humidity,  sound_dB);
		Delay_ms(100);
		
			}
    return 0;
}