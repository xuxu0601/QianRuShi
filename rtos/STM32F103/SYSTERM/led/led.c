#include "stm32f10x.h"
#include "led.h"
void LED_Init(void)
 {
		GPIO_InitTypeDef  GPIO_InitStructure;//�����ṹ�塪��GPIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʱ��ʹ��GPIOB
	 //RCC APB2PeriphClockcmd(RCC APB2Periph GPIOB | RCC APB2Periph AFIO,ENABLE); 
	 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;//��������pin_8 pin_9 pin_10 pin_11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//Ƶ��50Mhz
	GPIO_Init(GPIOC,&GPIO_InitStructure);//��ʼ��PA1-PA4,4����Ϩ��
	GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_2);//GPIO_Pin_3|GPIO_Pin_4
 }
 
 
 