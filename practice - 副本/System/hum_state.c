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
static uint8_t g_target_humi = 70;     // 默认目标湿度
static uint32_t g_shutdown_timer = 0;   // 关机倒计时
volatile uint32_t system_time_ms = 0;  // 确保在SysTick中断中更新


// SysTick初始化（1ms中断）
void SysTick_Init(void) {
    SysTick_Config(SystemCoreClock / 1000);  // 72MHz / 1000 = 72kHz → 1ms中断
}
 //SysTick中断服务函数
void SysTick_Handler1(void) {
    system_time_ms++;
}

// 初始化加湿器状态
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

// 更新状态（需在SysTick中断或主循环中调用）
void HumState_Update(void) {
   
    static uint32_t last_update = 0;
    uint32_t now = system_time_ms;
	
	if(g_current_state == HUM_MANUAL || HUM_SLEEP)
		FanAndWater_ON();
		
    
    // 关机模式下启动倒计时
    if (g_current_state == HUM_OFF) {
		FanAndWater_OFF();
        if (now - g_shutdown_timer >= 10000) { // 10秒后进入睡眠
            g_current_state = HUM_SLEEP;
             OLED_WriteCommand(0xAE);	//关闭显示
        }
    }
    
    // 睡眠模式下关闭显示并限制噪音
    if (g_current_state == HUM_SLEEP) {
        OLED_Clear();
        // 限制噪音：若噪声>32dB，降低PWM占空比
        if (sound_dB > 65.0f && control > 70.0f) {
        control = 70.0f;
    }

    // 更新PWM输出
    PWM_SetFanSpeed(control);
    }
}

// 设置目标湿度
void HumState_SetTargetHumi(uint8_t humi) {
    if (humi <= 100) {
        g_target_humi = humi;
        OLED_ShowNum(2, 8, (float)humi, 2); // 立即更新显示
    }
	
	
	
	
	
	
}

// 获取目标湿度
uint8_t HumState_GetTargetHumi(void) {
	
	
	
	
    return g_target_humi;
}

// 获取当前状态
HumidifierState HumState_GetCurrentState(void) {
    return g_current_state; // 直接返回状态，避免错误赋值
}

// 处理按键事件
void HumState_HandleKeyEvent(KeyEvent event) {
    switch (event) {
        case KEY_MODE_PRESSED:
            // 循环切换状态：HUM_MANUAL → HUM_SLEEP → HUM_OFF
            g_current_state = (g_current_state + 1) % 3;
            if (g_current_state == HUM_OFF) {
                g_shutdown_timer = system_time_ms; // 开始关机倒计时
				OLED_WriteCommand(0xAF);	//开启显示
                OLED_ShowString(2, 13, "OFF");
            } else if (g_current_state == HUM_SLEEP) {
				OLED_ShowString(2, 13, "SLE");
                OLED_WriteCommand(0xAE);	//关闭显示
            }
			else {
			OLED_ShowString(2, 13, "MAN");
			OLED_WriteCommand(0xAF);	//开启显示	
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




