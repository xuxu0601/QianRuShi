#include "stm32f10x.h"
#include  "key.h"
#include "delay.h"
void KEY_Init(void)
 {
	GPIO_InitTypeDef  GPIO_InitStructure;//�����ṹ�塪��GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʱ��ʹ��GPIOB
	 //RCC APB2PeriphClockcmd(RCC APB2Periph GPIOB | RCC APB2Periph AFIO,ENABLE); 
	 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//��������pin_8 pin_9 pin_10 pin_11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;    //��������ģʽ   Out_PP;//�������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//Ƶ��50Mhz
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��PA1-PA4,4����Ϩ��
 }
  u8 KEY_Scan(void) //��ֵ����
 {
		static u8 key_up=1;  //�����ɿ�״̬
	 if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	 {
		 delay_ms(100);   //����ȥ��
		 key_up=0;        //˵���бպ�
		 if(KEY1==0)
		 {
			 return 1;
		 }
		 else if(KEY2==0)
		 {
		 return 2;
		 }
		  //else if(KEY3==0&&KEY4==0) ͬʱ����Ӧ����
		 else if(KEY3==0)
		 {
		 return 3;
		 }
		 else if(KEY4==0)
		 {
		 return 4;
		 }
	 }
	 else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
		 key_up=1;
	 return 0;
}
 