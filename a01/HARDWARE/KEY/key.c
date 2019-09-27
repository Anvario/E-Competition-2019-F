
/*  key.c  */


#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"



u8 Key_Sta[4]={0};//按键的状态


//按键初始化函数
void Key_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(KEY0_RCC_CLK,ENABLE);//使能PORTA,PORTE时钟
	RCC_APB2PeriphClockCmd(KEY_GPIO_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	GPIO_InitStructure.GPIO_Pin=KEY0_GPIO_PIN|KEY1_GPIO_PIN|KEY2_GPIO_PIN|KEY3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_GPIO_PORT,&	GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY4_GPIO_PIN|KEY5_GPIO_PIN|KEY6_GPIO_PIN|KEY7_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_GPIO_PORT,&	GPIO_InitStructure);
}


//独立按键按键处理函数
//返回按键值
//0，没有任何按键按下
//1，KEY0按下

//mode  保留,随便传入一个值就OK
//
u8 Key_Scan(u8 mode)
{
	//key0
	if(Key_Sta[0]==0){
		if(KEY0_HARD_STA){
			delay_ms(10);
			if(KEY0_HARD_STA){
				Key_Sta[0]=1;
				return KEY0_PRES;
			}
		}
	}
	else{
		if(!(KEY0_HARD_STA)){
			Key_Sta[0]=0;
		}
	}
 	return 0;// 无按键按下
}

//获取按键的当前状态
u8 Key_GetSta(u8 key){
	if(key<=3){
		return Key_Sta[key];
	}
	return 0;
}


int Read_KeyVal(void)//实现矩阵键盘。返回值为，各按键的键值，此键值由用户自己定义
{
	int KeyVal=0;  //keyVal为最后返回的键值
  
	GPIO_Write(KEY_GPIO_PORT ,(KEY_GPIO_PORT ->ODR & 0xfff0 | 0xf)); //先让PD0到PD3全部输出高
	if((KEY_GPIO_PORT ->IDR & 0x00f0)==0x0000)  //如果，PD4到PD7全为0，则，没有键按下。此时，返回值为-1
	return -1;
	else 
	{
		delay_ms(100);//延时去抖动
		if((KEY_GPIO_PORT ->IDR & 0x00f0)==0x0000)  //如果，延时5ms后，PD4到PD7又全为0,则，刚才引脚的电位变化是抖动产生的.
		return -1;
	}
	GPIO_Write(KEY_GPIO_PORT ,(KEY_GPIO_PORT ->ODR & 0xfff0 | 0x1)); //让PD3到PD0输出二进制的0001.
	switch(KEY_GPIO_PORT ->IDR & 0x00f0) //对PD4到PD7的值进行判断,以输出不同的键值
	{
		case 0x0010: KeyVal=0x0D; break;
		case 0x0020: KeyVal=0x0C; break;
		case 0x0040: KeyVal=0x0B; break;
		case 0x0080: KeyVal=0x0A; break; 
	}
	GPIO_Write(KEY_GPIO_PORT ,(KEY_GPIO_PORT ->ODR & 0xfff0 | 0x2)); //让PD3到PD0输出二进制的0010. 
	switch(KEY_GPIO_PORT ->IDR & 0x00f0)//对PD4到PD7的值进行判断,以输出不同的键值。
		{
		case 0x0010: KeyVal=0x0F; break;
		case 0x0020: KeyVal=0x09; break;
		case 0x0040: KeyVal=0x06; break; 
		case 0x0080: KeyVal=0x03; break;  
		} 
	GPIO_Write(KEY_GPIO_PORT ,(KEY_GPIO_PORT ->ODR & 0xfff0 | 0x4)); //让PD3到PD0输出二进制的0100. 
	switch(KEY_GPIO_PORT ->IDR & 0x00f0)//对PD4到PD7的值进行判断，以输出不同的键值。	
	{
		case 0x0010: KeyVal=0x00; break;
		case 0x0020: KeyVal=0x08; break;
		case 0x0040: KeyVal=0x05; break;
		case 0x0080: KeyVal=0x02; break; 
	}
	GPIO_Write(KEY_GPIO_PORT ,(KEY_GPIO_PORT ->ODR & 0xfff0 | 0x8)); //让PD3到PD0输出二进制的1000. 
	switch(KEY_GPIO_PORT ->IDR & 0x00f0)//对PD4到PD7的值进行判断，以输出不同的键值。
	{
		case 0x0010: KeyVal=0x0E; break;
		case 0x0020: KeyVal=0x07; break;
		case 0x0040: KeyVal=0x04; break;
		case 0x0080: KeyVal=0x01; break;
	}
	return KeyVal;
		
}







