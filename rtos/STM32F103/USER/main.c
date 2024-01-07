/*************************************************************************************
STM32F103-30102:
	VCC<->3.3V
	GND<->GND
	SCL<->PB7
	SDA<->PB8
	 IM<->PB9
0.96inch OLED :
	VCC<->3.3V
	GND<->GND
	SCL<->PA5
	SDA<->PA6
	RST<->PA3
	DC<->PA4
	CS<->PA2
USB-TTL:
	5V<->5V
	GND<->GND
	RXD<->PA9
	TXD<->PA10
蜂鸣器:
  VCC<->3.3V
	I/O<->PA0
	GND<->GND


**************************************************************************************/
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "max30102.h" 
#include "myiic.h"
#include "algorithm.h"
#include "oled.h"
#include "pwm.h"
#include "stmflash.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "semphr.h"

#include <string.h>

/*******max30102任务***********/
#define MAX_TASK_STACK_SIZE  128*4
#define MAX_TASK_PRIO        2
TaskHandle_t MAX_task_handler;

/*******OLED任务***********/
#define OLED_TASK_STACK_SIZE  128
#define OLED_TASK_PRIO        2
TaskHandle_t OLED_task_handler;

/*******串口任务***********/
#define UART_TASK_STACK_SIZE  128
#define UART_TASK_PRIO         2
TaskHandle_t UART_task_handler;


uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;
#define MAX_BRIGHTNESS 255
u16 dis_hr=0,dis_spo2=0;
u8 str[50];

u16 flash_w[100] = {0x00};//写入数组
u16 flash_r[100] = {0x00};//读取FLASH数组
u8 oled_fla = 0;
u8 uart_fla = 0;
void dis_DrawCurve(u32* data,u8 x);




//任务
void MAX_Task(void *pvParameters)
{
	uint32_t un_min, un_max, un_prev_data;  
	int i;
	int j;
	int32_t n_brightness;
	float f_temp;
	u8 temp[6];
	u8 pwmduty = 0;
	un_min=0x3FFFF;
	un_max=0;
	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
  for(i=0;i<n_ir_buffer_length;i++)
  {
    while(MAX30102_INT==1);   //wait until the interrupt pin asserts   
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
		aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number           
        
    if(un_min>aun_red_buffer[i])
        un_min=aun_red_buffer[i];    //update signal min
    if(un_max<aun_red_buffer[i])
        un_max=aun_red_buffer[i];    //update signal max
   }
	 un_prev_data=aun_red_buffer[i];
   maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	 
	while(1)
	{
		i=0;
    un_min=0x3FFFF;
    un_max=0;
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
    for(i=100;i<500;i++)
    {
      aun_red_buffer[i-100]=aun_red_buffer[i];
      aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
      //update the signal min and max
      if(un_min>aun_red_buffer[i])
      un_min=aun_red_buffer[i];
      if(un_max<aun_red_buffer[i])
      un_max=aun_red_buffer[i];
    }
		//take 100 sets of samples before calculating the heart rate.
    for(i=400;i<500;i++)
    {
      un_prev_data = aun_red_buffer[i-1];
      while(MAX30102_INT == 1);
      max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
			aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
      if(aun_red_buffer[i]>un_prev_data)
      {
        f_temp=aun_red_buffer[i]-un_prev_data;
        f_temp/=(un_max-un_min);
        f_temp*=MAX_BRIGHTNESS;
        n_brightness-=(int)f_temp;
        if(n_brightness<0)
        n_brightness=0;
      }
      else
      {
        f_temp=un_prev_data-aun_red_buffer[i];
        f_temp/=(un_max-un_min);
        f_temp*=MAX_BRIGHTNESS;
        n_brightness+=(int)f_temp;
        if(n_brightness>MAX_BRIGHTNESS)
            n_brightness=MAX_BRIGHTNESS;
      }
			//send samples and calculation result to terminal program through UART
			if(ch_hr_valid == 1 && n_heart_rate<120)//**/ ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<120 && n_sp02<101
			{
				dis_hr = n_heart_rate;
				dis_spo2 = n_sp02;
			}
			else
			{
				dis_hr = 0;
				dis_spo2 = 0;
			}
			//uart_fla = 1;//发送1
		}
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
		//显示刷新
		if(dis_hr == 0 && dis_spo2 == 0)  //**dis_hr == 0 && dis_spo2 == 0
		{
			uart_fla = 2;//发送2
		}
		else
		{
			uart_fla = 3;//发送3
		}
		vTaskDelay(10);
		oled_fla = 1;
		vTaskDelay(10);
		flash_w[j++] = dis_hr;
		flash_w[j++] = dis_spo2;
		if(dis_hr > 120 || dis_spo2 < 60)
		{
			pwmduty+=5;
			if(pwmduty >100) pwmduty = 5;
			PWM_SetCompare1(pwmduty);
		}
		else
		{
			PWM_SetCompare1(100);
		}
		if(j > 100)
		{
			j = 0;
			STMFLASH_Write_only(FLASH_SAVE_ADDR,flash_w,100);
		}
		vTaskDelay(80);
	}
}

//任务2
void OLED_Task(void *pvParameters)
{

	while(1)
	{
		if(oled_fla == 1)
		{
		  OLED_ShowString(0,0,str,16);
		  OLED_Fill(0,23,127,63,0);
		  //红光在上，红外在下
		  dis_DrawCurve(aun_red_buffer,20);
		  dis_DrawCurve(aun_ir_buffer,0);
		  OLED_Refresh_Gram();//更新显示到OLED	
			oled_fla = 0;
		}
    vTaskDelay(30);
	}
}

void UART_Task(void *pvParameters)
{
	u8 m = 0;
	while(1)
	{
		if(uart_fla == 1)
		{
			printf("HR=%i, ", n_heart_rate); 
		  printf("HRvalid=%i, ", ch_hr_valid);
			printf("SpO2=%i, ", n_sp02);
			printf("SPO2Valid=%i\r\n", ch_spo2_valid);
			uart_fla = 0;
		}
		else if(uart_fla == 2)
		{
			sprintf((char *)str,"HR:--- SpO2:---\r\n");//**HR:--- SpO2:--- 
			uart_fla = 0;
		}
		else if(uart_fla == 3)
		{
			sprintf((char *)str,"HR:%3d SpO2:%3d\r\n",dis_hr,dis_spo2);//**HR:%3d SpO2:%3d 
			uart_fla = 0;
		}
		
		if(USART_RX_STA&0x8000)
		{			
	//	printf("%c %c %c %c %c %c",USART_RX_BUF[0],USART_RX_BUF[1],USART_RX_BUF[2],USART_RX_BUF[3],USART_RX_BUF[4],USART_RX_BUF[5]);
		 //u8 len=USART_RX_STA&0x3fff;
		 if(strcmp((char *)USART_RX_BUF,"FLASH") == 0)
		 {
			 STMFLASH_Read(FLASH_SAVE_ADDR,flash_r,100);//读取FLASH
		   
			 for(m = 0; m <  100;)
			 {
				 printf("HR:%3d,SpO2:%3d,HR:%3d,SpO2:%3d\r\n", flash_r[m++],flash_r[m++],flash_r[m++],flash_r[m++]);
			 }
			
		 }
	    USART_RX_STA = 0;
		}
	}
}



	


int main(void)
{ 
	//variables to calculate the on-board LED brightness that reflects the heartbeats
   NVIC_Configuration();
	delay_init();	    	 //延时函数初始化	
  
	uart_init(115200);	 	//串口初始化为115200
	
	printf("\r\n MAX30102  init  \r\n");
	
	PWM_Init();
	PWM_SetCompare1(100);
	
	max30102_init();

	OLED_Init();
	OLED_ShowString(0,0,"  initializing  ",16);
	OLED_Refresh_Gram();//更新显示到OLED	 
	
   
	
	xTaskCreate(OLED_Task,"OLED_Task",OLED_TASK_STACK_SIZE,NULL,OLED_TASK_PRIO,&OLED_task_handler);
	xTaskCreate(UART_Task,"UART_Task",UART_TASK_STACK_SIZE,NULL,UART_TASK_PRIO,&UART_task_handler);
	xTaskCreate(MAX_Task,"MAX_Task",MAX_TASK_STACK_SIZE,NULL,MAX_TASK_PRIO,&MAX_task_handler);
  //启动任务调度器
  vTaskStartScheduler();
	
	while(1)
	{
		 
	}
}





void dis_DrawCurve(u32* data,u8 x)
{
	u16 i;
	u32 max=0,min=262144;
	u32 temp;
	u32 compress;
	
	for(i=0;i<128*2;i++)
	{
		if(data[i]>max)
		{
			max = data[i];
		}
		if(data[i]<min)
		{
			min = data[i];
		}
	}
	
	compress = (max-min)/20;
	
	for(i=0;i<128;i++)
	{
		temp = data[i*2] + data[i*2+1];
		temp/=2;
		temp -= min;
		temp/=compress;
		if(temp>20)temp=20;
		OLED_DrawPoint(i,63-x-temp,1);
	}
}


