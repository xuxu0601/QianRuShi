#include "stm32f10x.h"
#include "led.h"
void LED_Init(void)
 {
		GPIO_InitTypeDef  GPIO_InitStructure;//声明结构体——GPIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//时钟使能GPIOB
	 //RCC APB2PeriphClockcmd(RCC APB2Periph GPIOB | RCC APB2Periph AFIO,ENABLE); 
	 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;//定义引脚pin_8 pin_9 pin_10 pin_11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//频率50Mhz
	GPIO_Init(GPIOC,&GPIO_InitStructure);//初始化PA1-PA4,4个灯熄灭
	GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_2);//GPIO_Pin_3|GPIO_Pin_4
 }
 
 
 