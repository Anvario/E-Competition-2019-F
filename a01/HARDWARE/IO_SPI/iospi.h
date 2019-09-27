/*   iospi.h   */

#ifndef __IO_SPI_H_
#define __IO_SPI_H_

#include "sys.h"


/********************* 模拟SPI管脚及时钟定义 **************************/
//GPIO定义
#define IO_SPI_CS_GPIO       GPIOB
#define IO_SPI_SCLK_GPIO     GPIOB
#define IO_SPI_MISO_GPIO     GPIOB
#define IO_SPI_MOSI_GPIO     GPIOB

#define IO_SPI_CS_PIN         GPIO_Pin_12
#define IO_SPI_SCLK_PIN       GPIO_Pin_13
#define IO_SPI_MISO_PIN       GPIO_Pin_14
#define IO_SPI_MOSI_PIN       GPIO_Pin_15
//时钟定义
#define IO_SPI_CS_RCC_CLK     RCC_APB2Periph_GPIOB
#define IO_SPI_SCLK_RCC_CLK   RCC_APB2Periph_GPIOB
#define IO_SPI_MISO_RCC_CLK   RCC_APB2Periph_GPIOB
#define IO_SPI_MOSI_RCC_CLK   RCC_APB2Periph_GPIOB
/****************************** end *********************************/


/********************* 模拟SPIGPIO操作定义 **************************/
#define IO_SPI_CS_H()     IO_SPI_CS_GPIO->BSRR = IO_SPI_CS_PIN     //CS管脚置高
#define IO_SPI_CS_L()     IO_SPI_CS_GPIO->BRR  = IO_SPI_CS_PIN     //CS管脚置低
#define IO_SPI_SCLK_H()   IO_SPI_SCLK_GPIO->BSRR = IO_SPI_SCLK_PIN //SCLK管脚置高
#define IO_SPI_SCLK_L()   IO_SPI_SCLK_GPIO->BRR  = IO_SPI_SCLK_PIN //SCLK管脚置低
#define IO_SPI_MOSI_H()   IO_SPI_MOSI_GPIO->BSRR = IO_SPI_MOSI_PIN //MOSI管脚置高
#define IO_SPI_MOSI_L()   IO_SPI_MOSI_GPIO->BRR  = IO_SPI_MOSI_PIN //MOSI管脚置低

//获取MISO管脚输入状态
#define IO_SPI_MISO_GET_IN()  ((IO_SPI_MISO_GPIO->IDR & IO_SPI_MISO_PIN)?(1):(0))
/****************************** end *********************************/


/************************* 模拟SPI函数声明 **************************/
void    IO_SPI_Init(void);                //IO模拟SPI相关初始化
uint8_t IO_SPI_Read_Byte(void);           //IO模拟SPI读取一个字节
void    IO_SPI_Write_Byte(uint8_t byte);  //IO模拟SPI写一个字节
uint8_t IO_SPI_RW_Byte(uint8_t byte);     //IO模拟SPI字节读写
/****************************** end *********************************/


























#endif


































