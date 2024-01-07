#include "stm32f10x.h"


void PWM_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;//����ȽϵĽṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʱ��ʹ��GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //����ʱ����
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE); //��ӳ��
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //���PA15���Թ���
	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_7;//��������pin_0|Pin_7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������ģʽ���ı����ſ���Ȩ��Ƭ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//Ƶ��50Mhz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM2);
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 100-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������20Ϊ2ms  //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  //psc
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OCStructInit(&TIM_OCInitStructure);//��StructInit����ʼֵ�������ɱ����﷨����
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//����Ƚϵ�ģʽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����Ƚϵļ���
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//����Ƚϵ�ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;//����Ƚ�����CCR������Ĵ���  //ռ�ձ�50%  ռ�ձ�10%   ռ�ձ�90%  
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);//�鿴���Ŷ����֪��,����ӳ��Ĭ��ֻ��PA0���
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	

	
	/*��ʽ��PWMƵ�ʣ�Freq = CK_PSC/(PSC+1)/(ARR+1)
					PWMռ�ձȣ�Doty = CCR/(ARR+1)
					PWM�ֱ��ʣ�Reso = 1/(ARR+1)   */
  TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}

void PWM_SetCompare1(uint16_t Compare1)//��������ͨ��1��ֵ��ӦTIM_OC1Init ���Ӷ��ı�CCRʵ��ռ�ձȱ仯
{
	TIM_SetCompare1(TIM2, Compare1);
	
}

void PWM_SetCompare2(uint16_t Compare2)//��������ͨ��1��ֵ��ӦTIM_OC2Init ���Ӷ��ı�CCRʵ��ռ�ձȱ仯
{
	TIM_SetCompare2(TIM3, Compare2);
	
}

