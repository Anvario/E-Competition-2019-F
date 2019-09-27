#ifndef __USART_H_
#define __USART_H_


#include "stdio.h"
#include "sys.h"

void USART1_Init(u32 bound);//串口1初始化函数
void USART2_Init(u32 bound);//串口2初始化函数
void USART3_Init(u32 bound);//串口3初始化函数
void UART4_Init (u32 bound);//串口4初始化函数
void UART5_Init (u32 bound);//串口5初始化函数
//串口发送一个字节
void USART_SendByte(USART_TypeDef *USART_COM,u8 c);
//串口发送数组函数
void USART_SendBuf(USART_TypeDef *USART_COM,unsigned char *buf,u16 len);
//串口发送字符串函数
void USART_SendString(USART_TypeDef *USART_COM,unsigned char *s);




















#endif


