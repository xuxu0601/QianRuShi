#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "stmflash.h"

u16 flash_w[10] = {0x55,0x67,0x89,0x45,0x23,0xa1,0x45,0xac,0xb5,0xfc};//写入数组
u16 flash_r[10] = {0x00};//读取FLASH数组

void inlt()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	
	//LED1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOC,GPIO_Pin_14);		 //IO口输出高电平
}

int main(void)
{	
	
	delay_init();	
  inlt();  
  STMFLASH_Write_only(FLASH_SAVE_ADDR,flash_w,10);
	delay_ms(100);
	STMFLASH_Read(FLASH_SAVE_ADDR,flash_r,10);//读取FLASH
	if(flash_r[9] == 0xfc)//如果成立，说明读取成功
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_14);		 //点亮LED，经过测试是点亮了
	}
	while(1)
	{
		
	}
}
