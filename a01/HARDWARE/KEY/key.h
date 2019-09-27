/*  key.h  */

#ifndef __KEY_H
#define __KEY_H
#include "sys.h"


//按键定义
#define KEY0  0


//key0 PB12   yuanlaiPE4



/********************* 按键所在GPIO口宏定义 ***************************/
#define KEY0_GPIO GPIOB
#define KEY0_PIN GPIO_Pin_12
/****************************** end *********************************/


/********************* 按键所在GPIO口时钟宏定义 *********************/
#define KEY0_RCC_CLK  RCC_APB2Periph_GPIOB
/****************************** end *********************************/



/********************* 按键硬件状态获取宏定义 ***********************/
#define KEY0_HARD_STA  ((KEY0_GPIO->IDR & (uint16_t)KEY0_PIN)==0)
/****************************** end *********************************/

 
 
/********************* 按键按下返回值宏定义 *************************/
#define KEY0_PRES 	1	//KEY0按下
/****************************** end *********************************/



/********************* 按键函数声明 *********************************/

void Key_Init(void);   //IO初始化
u8 Key_Scan(u8);  	   //按键扫描函数
u8 Key_GetSta(u8 key); //按键当前状态获取  1:已被按下  0:未被按下


/****************************** end *********************************/



#define KEY_ON  1
#define KEY_OFF 0

#define KEY0_GPIO_PIN GPIO_Pin_0
#define KEY1_GPIO_PIN GPIO_Pin_1
#define KEY2_GPIO_PIN GPIO_Pin_2
#define KEY3_GPIO_PIN GPIO_Pin_3
#define KEY4_GPIO_PIN GPIO_Pin_4
#define KEY5_GPIO_PIN GPIO_Pin_5
#define KEY6_GPIO_PIN GPIO_Pin_6
#define KEY7_GPIO_PIN GPIO_Pin_7

#define KEY_GPIO_PORT  GPIOC
#define KEY_GPIO_CLK   RCC_APB2Periph_GPIOC

void KEY_GPIO_InitStruct(void);
int Read_KeyVal(void);










#endif











