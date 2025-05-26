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
    PID_Init();           // 初始化PID控制器
    SysTick_Init(); 	// 初始化SysTick（在main.c中实现）
	Serial_Init();
	Key_Init();  // 初始化按键模块
	HumState_Init();
	
    while (1) {
		
		// 1. 扫描按键并处理事件
        KeyEvent event = Key_Scan();
        if (event != KEY_EVENT_NONE) {
            HumState_HandleKeyEvent(event);
        }
		// 3. 更新加湿器状态
        HumState_Update();
		
        Adc();            // 更新噪声和水位数据
        Dht11();          // 更新温湿度数据
		Send_Sensor_Data();
		RemoteCommand_Process(); // 新增命令处理
        // 执行PID控制（目标湿度85%RH，当前湿度humidity，噪声sound_dB）
		float target_hum = (float)HumState_GetTargetHumi();
		OLED_ShowNum(2, 8, target_hum, 2);
		PID_Run( target_hum,  humidity,  sound_dB);
		Delay_ms(100);
		
			}
    return 0;
}