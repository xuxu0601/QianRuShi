#include "stm32f10x.h"
#include "timer.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"


int main(void)
{		
		delay_init();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		LED_Init();
		BEEP_Init();
		TIM3_Init(4999,7199);//500ms
		while(1)
		{
			if(timer_state==1)
			{
				//GPIO_ResetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_2);
				LED1=!LED1;
				LED2=!LED2;
				LED3=!LED3;
				beep1=!beep1;
				/*LED2=0;
				LED3=0;
				beep1=0;
				//delay_ms(1000);
				//GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_2);
				LED1=1;
				LED2=1;
				LED3=1;
				beep1=1;*/
			//delay_ms(1000);
				timer_state = 0;
			}
		}
	 
}
