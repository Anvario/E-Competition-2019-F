/*  beep.c  */

#include "beep.h"

void Beep_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	
 RCC_APB2PeriphClockCmd(BEEP_GPIO_RCC_CLK, ENABLE);	 //使能PB,PE端口时钟
  
  BEEP_OFF();
  
 GPIO_InitStructure.GPIO_Pin = BEEP_PIN;				       //LED0端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);					   //根据设定参数初始化LED0
 //GPIO_SetBits(BEEP_GPIO,BEEP_PIN);						         //LED0输出高
}
 
