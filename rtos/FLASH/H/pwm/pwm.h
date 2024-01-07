#ifndef _PWM_H
#define _PWM_H
#include "sys.h"

void PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare1);
void PWM_SetCompare2(uint16_t Compare2);
#endif