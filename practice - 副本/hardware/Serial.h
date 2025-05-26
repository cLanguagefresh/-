#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void Send_Sensor_Data(void);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
void USART1_IRQHandler(void);
int Serial_RxAvailable(void);        // ����Ƿ������ݿɶ�
char Serial_ReadByte(void);           // ��������ȡһ���ֽ�
 //�����������
int Serial_RxAvailable(void);   // �����ջ������Ƿ�������
char Serial_ReadByte(void);     // �ӻ�������ȡһ���ֽ�

#endif
