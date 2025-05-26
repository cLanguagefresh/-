#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void LED_Init(void)
{
	
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
															//ʹ�ø�������ǰ���뿪��ʱ�ӣ����������Ĳ�����Ч
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;					//����ṹ�����
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIOģʽ����ֵΪ�������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;				//GPIO���ţ���ֵΪ��0������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO�ٶȣ���ֵΪ50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//����ֵ��Ĺ���������ݸ�GPIO_Init����
															//�����ڲ����Զ����ݽṹ��Ĳ���������Ӧ�Ĵ���
															//ʵ��GPIOA�ĳ�ʼ��
	
	/*��ѭ����ѭ�����ڵĴ����һֱѭ��ִ��*/
	
		/*����1��GPIO_ResetBits���õ͵�ƽ��GPIO_SetBits���øߵ�ƽ*/
		//GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)1);
		//Delay_ms(1000);
	//��PA0��������Ϊ�͵�ƽ
						//��ʱ500ms
		
					//����ֵ��Ĺ���������ݸ�GPIO_Init����
															//�����ڲ����Զ����ݽṹ��Ĳ���������Ӧ�Ĵ���
															//ʵ��GPIOA�ĳ�ʼ��
	
}

void LED_ON(void)
{
	
	/*����2��GPIO_WriteBit���õ�/�ߵ�ƽ����Bit_RESET/Bit_SETָ��*/
		//GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);		//��PA0��������Ϊ�͵�ƽ
		//Delay_ms(500);										//��ʱ500ms
		//GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET);			//��PA0��������Ϊ�ߵ�ƽ
		//Delay_ms(5000);										//��ʱ500ms
	  GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)0);
}

void LED_OFF(void)
{
	//GPIO_SetBits(GPIOB,GPIO_Pin_10);
}
