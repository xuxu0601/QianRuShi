
#ifndef __LED_H
#define __LED_H	 
//LED端口定义
//#define LED0 PAout(8)// PA8
#define LED1 PCout(1)// PC1	白
#define LED2 PCout(2)// PC2	灰色
#define LED3 PCout(3)// PC3 紫色

void LED_Init(void);//初始化
//void display(void);
//void displaybuffer(void);
		 				    
#endif
