#ifndef __HUM_STATE_H
#define __HUM_STATE_H

#include "KEY.h"  // 包含 Key.h 以获取 KeyEvent 类型定义

// 加湿器状态枚举
typedef enum {
    HUM_MANUAL,  // 手动模式
    HUM_SLEEP,   // 睡眠模式
    HUM_OFF      // 关机模式
} HumidifierState;

// 状态管理函数
void HumState_Init(void);
void HumState_Update(void);
void HumState_SetTargetHumi(uint8_t humi);
uint8_t HumState_GetTargetHumi(void);
HumidifierState HumState_GetCurrentState(void);

void SysTick_Init(void);
void SysTick_Handler(void);

// 处理按键事件
void HumState_HandleKeyEvent(KeyEvent event);
void parseJSON(const char* json, const char* key, int* value);
void HumState_HandleRemoteCommand(const char* command) ;

#endif /* __HUM_STATE_H */
