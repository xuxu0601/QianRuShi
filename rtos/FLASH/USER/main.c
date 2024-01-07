#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "stmflash.h"

u16 flash_w[10] = {0x55,0x67,0x89,0x45,0x23,0xa1,0x45,0xac,0xb5,0xfc};//д������
u16 flash_r[10] = {0x00};//��ȡFLASH����

void inlt()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	
	//LED1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOC,GPIO_Pin_14);		 //IO������ߵ�ƽ
}

int main(void)
{	
	
	delay_init();	
  inlt();  
  STMFLASH_Write_only(FLASH_SAVE_ADDR,flash_w,10);
	delay_ms(100);
	STMFLASH_Read(FLASH_SAVE_ADDR,flash_r,10);//��ȡFLASH
	if(flash_r[9] == 0xfc)//���������˵����ȡ�ɹ�
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_14);		 //����LED�����������ǵ�����
	}
	while(1)
	{
		
	}
}
