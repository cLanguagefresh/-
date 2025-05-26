#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "stdio.h"
#include "AD.h"



float sound_dB = 0.0f; 
float water_cm = 0.0f; 
#define NOISE_THRESHOLD    1.0f   // 噪声阈值电压（V）
#define SENSOR_SENSITIVITY 0.05f  // 灵敏度：50mV/dB
#define FILTER_WINDOW_SIZE 10 		//滑动窗口滤波

static uint16_t adc_buffer[FILTER_WINDOW_SIZE];
//static uint8_t buffer_index = 0;


/**
  * 函    数：AD初始化
  * 参    数：无
  * 返 回 值：无
  */
void AD_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//开启ADC1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	/*设置ADC时钟*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);						//选择时钟6分频，ADCCLK = 72MHz / 6 = 12MHz
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7   ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA0、PA1、PA2和PA3引脚初始化为模拟输入
	
	
	/*不在此处配置规则组序列，而是在每次AD转换前配置，这样可以灵活更改AD转换的通道*/
	
	/*ADC初始化*/
	ADC_InitTypeDef ADC_InitStructure;						//定义结构体变量
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//模式，选择独立模式，即单独使用ADC1
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据对齐，选择右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发，使用软件触发，不需要外部触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//连续转换，失能，每转换一次规则组序列后停止
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//扫描模式，失能，只转换规则组的序列1这一个位置
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//通道数，为1，仅在扫描模式下，才需要指定大于1的数，在非扫描模式下，只能是1
	ADC_Init(ADC1, &ADC_InitStructure);						//将结构体变量交给ADC_Init，配置ADC1
	
	/*ADC使能*/
	ADC_Cmd(ADC1, ENABLE);									//使能ADC1，ADC开始运行
	
	/*ADC校准*/
	ADC_ResetCalibration(ADC1);								//固定流程，内部有电路会自动执行校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
  * 函    数：获取AD转换的值
  * 参    数：ADC_Channel 指定AD转换的通道，范围：ADC_Channel_x，其中x可以是0/1/2/3
  * 返 回 值：AD转换的值，范围：0~4095
  */
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);	//在每次转换前，根据函数形参灵活更改规则组的通道1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//软件触发AD转换一次
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//等待EOC标志位，即等待AD转换结束
	return ADC_GetConversionValue(ADC1);					//读数据寄存器，得到AD转换的结果
}



int Adc(void)
{
	uint16_t  AD5, AD7;	//定义AD值变量
   
	
	/*显示静态字符串*/
	OLED_ShowString(3, 1, "Sou:");
	OLED_ShowString(4, 1, "Dep:");
	
	AD5 = AD_GetValue(ADC_Channel_5);		//单次启动ADC，转换通道1
		
	AD7 = AD_GetValue(ADC_Channel_7);		//单次启动ADC，转换通道2
	

	
		
	//SOUND	
	uint16_t adc_value_sound = AD5;  // 从AD模块获取声音传感器数据
    float voltage = (adc_value_sound / 4096.0f) * 3.3f;
     sound_dB =  (voltage - NOISE_THRESHOLD) / SENSOR_SENSITIVITY;
	// 示例：显示分贝值
	char db_str[16];
	sprintf(db_str, "%.1f", sound_dB); // 保留1位小数
	OLED_ShowString(3, 5, db_str);

	//water
	uint16_t adc_value_water = AD7;
     water_cm =  (adc_value_water / 4095.0f) * 5.0f; // 转换为水深
	// 示例：显示水位
	char water_str[16];
	sprintf(water_str, "%.1f", water_cm); // 保留1位小数
	OLED_ShowString(4, 5, water_str);


	if(water_cm <= 1)
		OLED_ShowString(4, 9,"low");				//显示通道1的转换结果AD1
	//	OLED_ShowNum(4, 9, AD7, 4);				//显示通道2的转换结果AD2
		

	
}

uint16_t AD_GetFilteredValue(uint8_t channel) {
    static uint16_t buffer[FILTER_WINDOW_SIZE];
    static uint8_t index = 0;
    buffer[index] = AD_GetValue(channel);
    index = (index + 1) % FILTER_WINDOW_SIZE;
    uint32_t sum = 0;
    for (uint8_t i = 0; i < FILTER_WINDOW_SIZE; i++) sum += buffer[i];
    return sum / FILTER_WINDOW_SIZE;
}

