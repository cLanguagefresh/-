#ifndef __AD_H
#define __AD_H

#define NOISE_THRESHOLD    1.0f   // ������ֵ��ѹ��V��
#define SENSOR_SENSITIVITY 0.05f  // �����ȣ�50mV/dB
#define FILTER_WINDOW_SIZE 10

extern float sound_dB;
extern float water_cm;

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
int Adc(void);

uint16_t AD_GetFilteredValue(uint8_t channel);

#endif
