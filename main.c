#include "VI.h"
#include "SPI.h"
#include "delay.h"
#include <stdio.h>
#include "ILI9341.h"
#define volt_pin 0 		  //connect to A0
#define Current_pin 1   //connect to A1
float variable;
#define size 8
uint32_t x;
char data[size];
int main(void)
{
SystemCoreClockUpdate();
systick_init();
ADC_init();
ILI9341_Init();
ILI9341_setRotation(2);
ILI9341_Fill(COLOR_BLACK);

//ILI9341_DrawString(0,0,"AC Power Analyzer",COLOR_WHITE,COLOR_BLACK,2);
//ILI9341_DrawString(0,1,"STM32 Developers",COLOR_WHITE,COLOR_BLACK,2);
//delay(3000);
ILI9341_Fill(COLOR_BLACK);

while(1)
{
	

calcVI(1000,100,0,490,18);

sprintf(data,"V=%f Vrms",getVrms());
ILI9341_DrawString(0,0,data,COLOR_WHITE,COLOR_BLACK,2);


sprintf(data,"I=%f Irms",getIrms());
ILI9341_DrawString(0,1,data,COLOR_WHITE,COLOR_BLACK,2);


sprintf(data,"PF=%f",getpowerFactor());

ILI9341_DrawString(0,2,data,COLOR_WHITE,COLOR_BLACK,2);	
	
	
double theta=asin(getpowerFactor());
double Q=getapparentPower()*theta;

sprintf(data,"Q=%f  VAr",-1*Q);
ILI9341_DrawString(0,3,data,COLOR_WHITE,COLOR_BLACK,2);
	
sprintf(data,"P=%f  W",getRealPower());

ILI9341_DrawString(0,4,data,COLOR_WHITE,COLOR_BLACK,2);

sprintf(data,"S=%f  VA",getapparentPower());
ILI9341_DrawString(0,5,data,COLOR_WHITE,COLOR_BLACK,2);
//ILI9341_Fill(COLOR_BLACK);

}	
	
	
}
