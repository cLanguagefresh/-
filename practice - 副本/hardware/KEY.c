#include "stm32f10x.h"                  // Device header
#include "KEY.h"
#include "Delay.h"
#include "OLED.h"

void Key_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    // 使能 GPIOA 和 GPIOB 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 配置 PA15、PA12、PA11 为下拉输入（按下时高电平）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_12 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;  // 下拉输入
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置 PB8、PB9 为推挽输出（控制继电器）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 初始化继电器为关闭状态（输出低电平）
    GPIO_ResetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
}


KeyEvent Key_Scan(void) {
    // 检测模式键（PA15，按下时高电平）
    if (GPIO_ReadInputDataBit(GPIOA, MODE_KEY_PIN) == Bit_SET) {
        Delay_ms(20); // 消抖
        if (GPIO_ReadInputDataBit(GPIOA, MODE_KEY_PIN) == Bit_SET) {
            while (GPIO_ReadInputDataBit(GPIOA, MODE_KEY_PIN) == Bit_SET); // 等待释放
			OLED_ShowString(4, 15, "M");
            return KEY_MODE_PRESSED;
        }
    }

    // 检测湿度-键（PA12，按下时高电平）
    if (GPIO_ReadInputDataBit(GPIOA, DEC_HUMI_PIN) == Bit_SET) {
           Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOA, DEC_HUMI_PIN) == Bit_SET) {
            while (GPIO_ReadInputDataBit(GPIOA, DEC_HUMI_PIN) == Bit_SET);
			OLED_ShowString(4, 15, "-");
            return KEY_DEC_HUMI_PRESSED;
        }
    }

    // 检测湿度+键（PA11，按下时高电平）
    if (GPIO_ReadInputDataBit(GPIOA, INC_HUMI_PIN) == Bit_SET) {
        Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOA, INC_HUMI_PIN) == Bit_SET) {
            while (GPIO_ReadInputDataBit(GPIOA, INC_HUMI_PIN) == Bit_SET)
			OLED_ShowString(4, 15, "+");
            return KEY_INC_HUMI_PRESSED;
        }
    }

    return KEY_EVENT_NONE;
}

void FanAndWater_ON(void)
{
	 GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction) 1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction) 1);
	
}

void FanAndWater_OFF(void)
{
	 GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction) 0);
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction) 0);
	
}
