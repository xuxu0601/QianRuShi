#include "stm32f10x.h"
#include "beep.h"
void BEEP_Init(void)
 {
	GPIO_InitTypeDef  GPIO_InitStructure;//�����ṹ�塪��GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʱ��ʹ��GPIOB
	 //RCC APB2PeriphClockcmd(RCC APB2Periph GPIOB | RCC APB2Periph AFIO,ENABLE); 
	 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//��������pin_8 pin_9 pin_10 pin_11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//Ƶ��50Mhz
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��PA1-PA4,4����Ϩ��
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//GPIO_Pin_3|GPIO_Pin_4
 }
 
 
 