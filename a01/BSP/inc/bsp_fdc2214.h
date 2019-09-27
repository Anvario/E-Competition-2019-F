#ifndef __BSP_FDC2214_H
#define __BSP_FDC2214_H

void InitSingleFDC2214(void);
void InitMultiFDC2214(void);//Ë«Í¨µÀ
unsigned int ReadRegfdc2214(unsigned char add);
int FDC2X14ReadCH(unsigned char index);
unsigned int FDCRead(unsigned char index);

#endif
