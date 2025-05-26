#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);
void PWM_SetCompare2(uint16_t Compare);
void PWM_FAN(void);
void PWM_SetFanSpeed(float duty);


#endif
