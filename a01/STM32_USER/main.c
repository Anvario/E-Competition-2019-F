/*  main.c  */

//Board Support Package
#include "bsp.h" 

//Definition
#define A -965.25994
#define B -440.92482
#define C 0.17401
#define AA 348.66222
#define BB -222.17895
#define CC 0.20174

#define N 100+1
int standardA[N] = {0,
		825,  1081, 1074, 1327, 1412,  //1-5
		1486, 1546, 1634 ,  1685 ,  1713 ,  //6-10
	
		1766,  1805,  1832,  1870,  1897,  //11-15
    1937,  1960,  1983,  2001,  2020,  //16-20
	
    2041,  2056,  2076,  2092,  2106,  //21-25
    2120,  2133,  2148,  2162,  2172,  //26-30
};


int standardB[N] = {0,
	423,  540,  625,  680,  724,     //1-5
	764,  793,  826,  853,  876,    //6-10
	
	899,  918,  933,  952,  969,   //11-15
	984,  998,  1008,  1020, 1031,  //16-20
	
	1042, 1051, 1059, 1067, 1076,  //21-25
	1085, 1090, 1097, 1105, 1113,  //26-30
	
} ;

int chValue = 0;
int chValueSeq = 0;
int temp[200] = {0};
int tempSeq[200] = {0};
int tempSum = 0;
int tempSeqSum = 0;
int fixError[101] = {0};		//自校准误差
int fixSeqError[101] = {0};
int flag=0;

double getValue(int key);
int getKey(double value, double refe);
int getKey2(double value);
double getVValue(int key);
int getKKey(double value);
void fix (void);
void alarm(void);
int getB(int);
int getA(int);

int book[3000]={0};
int cook[3000]={0};
int bookSeq[2000]={0};
int cookSeq[2000]={0};
int i;
int a2=0,a1=0,n=0,up=0;

//Main
int main(void)
{
	for(i=0;i<3000;i++){
		book[i] = i-1500;
		cook[i] = getKey2(i-1500);
	}
	for(i=0;i<2000;i++){
		bookSeq[i] = i;
		cookSeq[i] = getKKey(i);
	}
	delay_init();	    //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LED_Init();
	USART1_Init(115200);//串口1初始化
	LCD_Init();	
	Key_Init();
	Beep_Init();
	IIC_Init();
	InitMultiFDC2214();
	
	POINT_COLOR = BLACK;
	LCD_ShowString(20,150,220,24,24,"FDC2214 Test !");//显示字符串
	delay_ms(1000);
	LCD_Fill(20,150,20+220,150+24,WHITE);
	while(1)
	{
		//自校准
		if(Read_KeyVal()==0x0A){		
			delay_ms(20);
			if(Read_KeyVal()==0x0A){
				fix();
			}
			LCD_Clear(WHITE);
		}
		printf("%d\n",Read_KeyVal());
		

		if(FDCRead(2)/10000>7000){
			LCD_Clear(WHITE);
			POINT_COLOR = RED;
			LCD_ShowString(20,24+24*5,220,24,24,"Short_Circuit!");//显示0张警告
			alarm();
			
			POINT_COLOR = BLACK;
			LCD_ShowString(20,24+24*5,220,24,24,"Short_Circuit!");//显示0张警告
			alarm();
			LCD_ShowString(20,24+24*5,220,24,24,"              ");//显示0张警告
		}
		//测试
		if(Key_Scan(0)&&KEY0_PRES==1){	
				LCD_Clear(WHITE);
				
						
				//消噪、读数		
				tempSum = 0;
				tempSeqSum = 0;
				
				for (i=0;i<200;i++){
					temp[i] = FDC2X14ReadCH(2)/10000;
					tempSeq[i] = FDCRead(2)/10000;
					tempSum += temp[i];
					tempSeqSum += tempSeq[i];
					delay_ms(8);	
				}
				chValue = tempSum/200.0; 
				chValueSeq = tempSeqSum/200.0; 
				
				if (chValue>700&&chValue<2000&&chValueSeq<1000){
				    chValue -= 1677;
  			    }
				chValue += 1677;
				
				printf("%d\n",chValue);
				BEEP_ON();
				LED_TOGGLE(LED1);
				//显示
//				LCD_ShowString(20,24-24,220,24,24,"Signnal1:");//显示信号值
//				LCD_ShowNum(20,24,chValue,15,24);
				LCD_ShowString(20,24+24,220,24,24,"Signnal:");//显示信号值
				LCD_ShowNum(20,24+24*2,chValueSeq,15,24);

//				if (chValue>7000){
//					LCD_ShowString(20,24+24*3,220,24,24,"Short_Circuit!");//显示0张警告
//					LCD_ShowNum(20,24+24*4,0,15,24);					
//					alarm();
//				}else{
//					LCD_ShowString(20,24+24*3,220,24,24,"Paper_Number1:");//显示纸张数量
//					LCD_ShowNum(20,24+24*4,getA(chValue),15,24);
//				}
				if(chValueSeq>7000){
					LCD_ShowString(20,24+24*5,220,24,24,"Short_Circuit!");//显示0张警告
					LCD_ShowNum(20,24+24*6,0,15,24);		
					alarm();
				}else{
					LCD_ShowString(20,24+24*5,220,24,24,"Paper_Number2:");//显示纸张数量
					LCD_ShowNum(20,24+24*6,getB(chValueSeq),15,24);
				}
				BEEP_OFF();
				LED_TOGGLE(LED1);
		}
	}
}

double getValue(int key){
	return A - B * log( (double)key + C );
}

int getKey(double value,double refe){   //溢出矫正
	if (value>8000){    
		return 0;
	}
	if (value>700&&value<2000&&refe<1000){
		return (int)( (exp( (A-(value-1677))/B)-C)+0.5 );
	}
	return (int)( (exp( (A-value)/B)-C)+0.5 );
}
int getKey2(double value){		//无溢出矫正
	if (value>8000){    
		return 0;
	}
	return (int)( (exp( (A-value)/B)-C)+0.5 );
}

double getVValue(int key){
	return AA - BB * log( (double)key + CC );
}

int getKKey(double value){
	if (value>8000){
		return 0;
	}
	return (int)( (exp( (AA-value)/BB)-CC)+0.5 );
}

void fix(){
	while(1){
		BEEP_ON();
		delay_ms(100);		
		BEEP_OFF();
		LCD_Clear(WHITE);
		LCD_ShowString(20,24+24*(-1),220,24,24,"     Fix:");
		delay_ms(1000);
		while(Read_KeyVal()==-1){}
		if(Read_KeyVal()>=0&&Read_KeyVal()<=9){
			a2 = Read_KeyVal();
			BEEP_ON();
			delay_ms(200);		
			BEEP_OFF();
			delay_ms(800);
			while(Read_KeyVal()==-1){}
			while(Read_KeyVal()>=0&&Read_KeyVal()<=9){
				a1 = Read_KeyVal();
				delay_ms(500);	
				n = 10*a2 + a1;
				BEEP_ON();
				delay_ms(200);		
				BEEP_OFF();
			}
		}
		
		LCD_ShowNum(20+24*5,24+24*0,n,6,24);
		while(Read_KeyVal()==-1){}
		if(Read_KeyVal()==0x0E){
			BEEP_ON();
			delay_ms(200);		
			BEEP_OFF();
			continue;
		}else if(Read_KeyVal()==0x0B){
			BEEP_ON();
			delay_ms(200);		
			BEEP_OFF();
			break;
		}else if(Read_KeyVal()==0x0F){
			BEEP_ON();
			delay_ms(200);		
			BEEP_OFF();		
			//消噪、读数		
			tempSum = 0;
			tempSeqSum = 0;
		
			for (i=0;i<200;i++){
				temp[i] = FDC2X14ReadCH(2)/10000;
				tempSeq[i] = FDCRead(2)/10000;
				tempSum += temp[i];
				tempSeqSum += tempSeq[i];
				delay_ms(5);	
			}
			
			//实测值
			chValue = tempSum/200;											
			chValueSeq = tempSeqSum/200;
			
			//赋值
			standardA[n] = chValue;
			standardB[n] = chValueSeq;
			//求误差
//			fixError[n] = abs(chValue - standardA[n]);
//			fixSeqError[n] =  abs(chValueSeq - standardB[n]);
			delay_ms(200);	
//			LCD_ShowNum(20,24+24*1,chValue,15,24);
//			delay_ms(200);	
			LCD_ShowNum(20,24+24*2,chValueSeq,15,24);
			delay_ms(200);	
			LCD_ShowString(20,24+24*(3),220,24,24,"OK");
		}
		while(Read_KeyVal()==-1){}
		if(Read_KeyVal()==0x0C){
			BEEP_ON();
			delay_ms(200);		
			BEEP_OFF();
			LCD_Clear(WHITE);
			LCD_ShowString(20,24+24*(-1),220,24,24,"Auto Fix:");
			for (up=0; up<=100; up++){
				delay_ms(200);
				LCD_ShowNum(20+24*5,24+24*0,up,6,24);
				
				while(Read_KeyVal()==-1){}
				while(Read_KeyVal()!=0x0D){
					if(Read_KeyVal()!=0x0B){
						flag = 1;
					}
				}
					if(flag){
						break;
					}
					delay_ms(200);
					BEEP_ON();
					delay_ms(200);		
					BEEP_OFF();
					tempSum = 0;
					tempSeqSum = 0;
					for (i=0;i<200;i++){
						temp[i] = FDC2X14ReadCH(2)/10000;
						tempSeq[i] = FDCRead(2)/10000;
						tempSum += temp[i];
						tempSeqSum += tempSeq[i];
						delay_ms(8);	
					}
					//实测值
					chValue = tempSum/200;											
					chValueSeq = tempSeqSum/200;
					//赋值
					standardA[up] = chValue;
					standardB[up] = chValueSeq;	
					LCD_ShowNum(20,24+24*1,chValueSeq,15,24);
					LCD_ShowString(20,24+24*(3),220,24,24,"OK");

			}
		}
		if(Read_KeyVal()==0x0B){
			break;
		}
	}
	LCD_Clear(WHITE);
	flag = 0;
}

void alarm(){
	BEEP_ON();
	delay_ms(200);		
	BEEP_OFF();
	delay_ms(200);
	BEEP_ON();
	delay_ms(200);		
	BEEP_OFF();
	delay_ms(200);
	BEEP_ON();
	delay_ms(200);		
	BEEP_OFF();
}

int getA(int chValue){	   
	int errorA[N] = {0};	
	int tempA=9999;
	int targetA=0; 
  for(i=1;i<N;i++){
		errorA[i] = abs(chValue-standardA[i]);
		if(errorA[i]<tempA){
  		tempA = errorA[i];
  		targetA = i;
		}			  
	}			
	return targetA;
}
int getB(int chValueSeq){	
	int errorB[N] = {0};
	int tempB=9999;
	int targetB=0; 
  for(i=1;i<N;i++){
		errorB[i] = abs(chValueSeq-standardB[i]);
		if(errorB[i]<tempB){
  		tempB = errorB[i];
  		targetB = i;
		}			  
	}			
	return targetB;
}
