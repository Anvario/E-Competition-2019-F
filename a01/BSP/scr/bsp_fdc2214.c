#include "bsp.h"


#define FDC2X14_Address 0x2A						
#define FDC2X14_W FDC2X14_Address<<1
#define FDC2X14_R (FDC2X14_Address<<1)+1
#define DATA_CH0 0x00          //通道0数据的高12位
#define DATA_LSB_CH0 0x01				//通道1数据的低12位，位于数据手册15页，表4中
#define DATA_CH1 0x02
#define DATA_LSB_CH1 0x03
#define DATA_CH2 0x04
#define DATA_LSB_CH2 0x05
#define DATA_CH3 0x06
#define DATA_LSB_CH3 0x07
#define RCOUNT_CH0 0x08 			//转换时间配置寄存器，
#define RCOUNT_CH1 0x09
#define RCOUNT_CH2 0x0A
#define RCOUNT_CH3 0x0B
#define OFFSET_CH0 0x0C
#define OFFSET_CH1 0x0D
#define OFFSET_CH2 0x0E
#define OFFSET_CH3 0x0F
#define SETTLECOUNT_CH0 0x10		//通道建立时间配置寄存器，位于数据手册16页，表5
#define SETTLECOUNT_CH1 0x11
#define SETTLECOUNT_CH2 0x12
#define SETTLECOUNT_CH3 0x13
#define CLOCK_DIVIDERS_C_CH0 0x14
#define CLOCK_DIVIDERS_C_CH1 0x15
#define CLOCK_DIVIDERS_C_CH2 0x16
#define CLOCK_DIVIDERS_C_CH3 0x17
#define STATUS 0x18
#define ERROR_CONFIG 0x19
#define CONFIG 0x1A
#define MUX_CONFIG 0x1B
#define RESET_DEV 0x1C
#define DRIVE_CURRENT_CH0 0x1E
#define DRIVE_CURRENT_CH1 0x1F
#define DRIVE_CURRENT_CH2 0x20
#define DRIVE_CURRENT_CH3 0x21
#define MANUFACTURER_ID 0x7E
#define DEVICE_ID 0x7F
unsigned int ReadFDC2X14(u8 firstAddress,u8 secondAddress);
unsigned int FDCRead(unsigned char index);
/*******************************************************************************
* 函数名	: WriteRegfdc2214 
* 描述	    : 写寄存器数据
* 输入参数  : add，value
* 返回参数  : 无
*******************************************************************************/
void WriteRegfdc2214(u8 add,u16 value)
{
    IIC_Start();  
    IIC_Send_Byte(0X54);	 //ADDR=0时，地址0X2A<<1+0=0X54  
    IIC_Wait_Ack();          //等应答
    IIC_Send_Byte(add);      //写地址
    IIC_Wait_Ack();          //等应答
    IIC_Send_Byte(value>>8); //写高8位
    IIC_Wait_Ack();
    IIC_Send_Byte(value&0xff);//写低8位
    IIC_Wait_Ack();
    IIC_Stop();               //产生一个停止条件 
    delay_ms(10);	 
}
/*******************************************************************************
* 函数名	: ReadRegfdc2214 
* 描述	    : 读寄存器数据
* 输入参数  : add地址
* 返回参数  : 无
*******************************************************************************/
unsigned int ReadRegfdc2214(unsigned char add)
{
    u16 status;
    u16 a,b;
    IIC_Start(); 
    IIC_Send_Byte(0X54);	   //写命令ADDR=0
    IIC_Wait_Ack();
    IIC_Send_Byte(add);     //地址
    IIC_Wait_Ack();
    IIC_Start();            //重新开始
    IIC_Send_Byte(0X55);	   //发送读命令ADDR=0
    IIC_Wait_Ack(); 
    a=IIC_Read_Byte(1);     //读高位
    b=IIC_Read_Byte(0);		 //读低位
    status=(a<<8)+b;
    IIC_Stop();
    return (status);
}


//自定义函数
unsigned int FDCRead(unsigned char index) 
{
	u32 result;
	switch(index)
	{
		case 1:
		  result = ReadRegfdc2214(DATA_CH0)&0x0FFF;
		  result = (result<<16)|(ReadRegfdc2214(DATA_LSB_CH0));
			break;
		case 2:
			result = ReadRegfdc2214(DATA_CH1)&0x0FFF;
		  result = (result<<16)|(ReadRegfdc2214(DATA_LSB_CH1));
			break;
		case 3:
			result = ReadRegfdc2214(DATA_CH2)&0x0FFF;
		  result = (result<<16)|(ReadRegfdc2214(DATA_LSB_CH2));
			break;
		case 4:
			result = ReadRegfdc2214(DATA_CH3)&0x0FFF;
		  result = (result<<16)|(ReadRegfdc2214(DATA_LSB_CH3));
			break;
		default:break;
	}
	result =result&0x0FFFFFFF;
	return result;
}


//数据读取
//index通道索引
int FDC2X14ReadCH(u8 index)
{
	int result;
	switch(index)
	{
		case 0x01:
			result = ReadFDC2X14(DATA_CH0,DATA_LSB_CH0);
			break;
		case 0x02:
			result = ReadFDC2X14(DATA_CH1,DATA_LSB_CH1);
			break;
		case 0x03:
			result = ReadFDC2X14(DATA_CH2,DATA_LSB_CH2);
			break;
		case 0x04:
			result = ReadFDC2X14(DATA_CH3,DATA_LSB_CH3);
			break;
	}
	return result;
}
unsigned int ReadFDC2X14(u8 firstAddress,u8 secondAddress)
{
	 unsigned int temp;
  u8 result[4];
  IIC_Start(); 
  IIC_Send_Byte(FDC2X14_W);
  IIC_Wait_Ack();
	IIC_Send_Byte(firstAddress);
	IIC_Wait_Ack();
	IIC_Stop();
	
	IIC_Start();
	IIC_Send_Byte(FDC2X14_R);
  IIC_Wait_Ack();
  result[0]=IIC_Read_Byte(1);//高字节 高四位清除
	result[0] = result[0]<<4;
	result[0] = result[0]>>4;
  IIC_Ack(); 
  result[1]=(IIC_Read_Byte(1));          //temp=高字节+低字节
	IIC_Ack(); 
	IIC_Stop();
	
	IIC_Start(); 
  IIC_Send_Byte(FDC2X14_W);
  IIC_Wait_Ack();
	IIC_Send_Byte(secondAddress);
	IIC_Wait_Ack();
	IIC_Stop();

	
	IIC_Start();
	IIC_Send_Byte(FDC2X14_R);
  IIC_Wait_Ack();
  
  result[2]=IIC_Read_Byte(1);//高字节 高四位清除
  IIC_Ack(); 
	
  result[3]=(IIC_Read_Byte(1));          //temp=高字节+低字节
	IIC_Ack(); 
	IIC_Stop();
  temp = (unsigned int)(((result[0]<< 24) | (result[1] << 16) | (result[2] << 8) | (result[3] & 0xff)));  
  return(temp); 
}

void SetFDC2X14(u8 Address,u8 MSB,u8 LSB)
{
	IIC_Start(); 
  IIC_Send_Byte(FDC2X14_W);
  IIC_Wait_Ack();

	IIC_Send_Byte(Address);
	IIC_Wait_Ack();

	IIC_Send_Byte(MSB);
	IIC_Wait_Ack();

	IIC_Send_Byte(LSB);
	IIC_Wait_Ack();

	IIC_Stop();
}

/*******************************************************************************
* 函数名	: InitSingleLDC1314 
* 描述	    : 初始化单通道
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void InitSingleFDC2214(void)
{
    WriteRegfdc2214(0x08,0xFFFF);//设置转换时间  设置达到最高精度

    WriteRegfdc2214(0x10,0x0064);//FDC2214_SETTLECOUNT_CH0 计数

    WriteRegfdc2214(0x14,0x2001); //分频洗系数
    //    LDC_Write_Reg(0x0C,0x0F00); //调零
    WriteRegfdc2214(0x19,0x0000); //ERROE_CONFIG
	
    WriteRegfdc2214(0x1B,0x020D); //通道配置
    WriteRegfdc2214(0x1E,0xF800); //配置驱动电流
    WriteRegfdc2214(0x1A,0x1C81); //参数配置
}
/*******************************************************************************
* 函数名	: InitMultiLDC1314 
* 描述	    : 初始化多通道
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void InitMultiFDC2214(void)//双通道
{
    WriteRegfdc2214(0x08,0x04D6);//转换时间
  	WriteRegfdc2214(0x09,0x04D6);
	WriteRegfdc2214(0x0A,0x04D6);//转换时间
  	WriteRegfdc2214(0x0B,0x04D6);
	
	WriteRegfdc2214(0x0C,0x0F00); //调0值 	CH0
	WriteRegfdc2214(0x0D,0x0F00); //调0值   CH1
	WriteRegfdc2214(0x0E,0x0F00); //调0值   CH2
	WriteRegfdc2214(0x0F,0x0F00); //调0值   CH3
  	
	WriteRegfdc2214(0x10,0x000A);//作用时间 CH1 
  	WriteRegfdc2214(0x11,0x000A);//CH2
	WriteRegfdc2214(0x12,0x000A);//CH3
	WriteRegfdc2214(0x13,0x000A);//CH4
  	
	WriteRegfdc2214(0x14,0x2002);//分频
  	WriteRegfdc2214(0x15,0x2002);
    WriteRegfdc2214(0x16,0x2002);//分频
  	WriteRegfdc2214(0x17,0x2002);
	
  	WriteRegfdc2214(0x19,0x0000);
  	WriteRegfdc2214(0x1B,0xC20D);//配置多通道   2通道--0x820D
	
  	WriteRegfdc2214(0x1E,0x9000); //驱动电流 CH0
  	WriteRegfdc2214(0x1F,0x9000); //CH1
		WriteRegfdc2214(0x20,0x9000); //CH2
  	WriteRegfdc2214(0x21,0x9000); //CH3
	WriteRegfdc2214(0x1A,0x1C81);//配置寄存器
}
	//Configuration register
	//	Active channel Select: b00 = ch0; b01 = ch1; b10 = ch2; b11 = ch3;
	//  |Sleep Mode: 0 - device active; 1 - device in sleep;
	//  ||Reserved, reserved, set to 1
	//  |||Sensor Activation Mode: 0 - drive sensor with full current. 1 - drive sensor with current set by DRIVE_CURRENT_CHn 
	//  ||||Reserved, set to 1
	//  |||||Reference clock: 0 - use internal; 1 - use external clock
	//  ||||||Reserved, set to 0
	//  |||||||Disable interrupt. 0 - interrupt output on INTB pin; 1 - no interrupt output
	//  ||||||||High current sensor mode: 0 - 1.5mA max. 1 - > 1.5mA, not available if Autoscan is enabled
	//  |||||||||     Reserved, set to 000001
	//  |||||||||     |
	// CCS1A1R0IH000000 -> 0001 1100 1000 0001 -> 0x1E81      0001 1100 1000 0001 -> 0x1C81


