#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

// 按键引脚定义
#define MODE_KEY_PIN    GPIO_Pin_15   // PA15: 模式键
#define DEC_HUMI_PIN    GPIO_Pin_12   // PA12: 湿度-
#define INC_HUMI_PIN    GPIO_Pin_11   // PA11: 湿度+

// 按键事件类型
 typedef enum {
    KEY_EVENT_NONE,
    KEY_MODE_PRESSED,     // 模式键按下
    KEY_DEC_HUMI_PRESSED, // 湿度-按下
    KEY_INC_HUMI_PRESSED  // 湿度+按下
} KeyEvent;

// 函数声明
void Key_Init(void);
KeyEvent Key_Scan(void);
void FanAndWater_ON(void);
void FanAndWater_OFF(void);

#endif /* __KEY_H */
