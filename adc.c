#include "stm32f4xx.h"                  // Device header
#include "adc.h"
#include "delay.h"
uint16_t adcValue1,adcValue2;

#define volt_pin 0 		  //connect to A0
#define Current_pin 1   //connect to A1
void ADC_init(void){
RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN; //enable gpio a clock
RCC->APB2ENR|=RCC_APB2ENR_ADC1EN|RCC_APB2ENR_ADC2EN; //enable adc clock
GPIOA->MODER|=0b00001111;  //set the PA0 & PA1 to analog mode
ADC1->CR2=0; //disable the adc
ADC1->SMPR2|=0b00111111;
ADC1->SQR3=0;
ADC1->CR2|=ADC_CR2_CONT;
ADC1->CR2|=1; //enable the adc
ADC1->CR2|=ADC_CR2_SWSTART;	

ADC2->CR2=0;
ADC2->SMPR2|=0b00111111;
ADC2->SQR3=1;

ADC2->CR2|=ADC_CR2_CONT;
ADC2->CR2|=1; //enable the adc
ADC2->CR2|=ADC_CR2_SWSTART;		

}


uint16_t readADCVolt(void)
{

while(!(ADC1->SR)&ADC_SR_EOC){;}
adcValue1=ADC1->DR;
return adcValue1 ;
}
	
uint16_t readADCCurrent(void)
{

while(!(ADC2->SR)&ADC_SR_EOC){;}
adcValue2=ADC2->DR;
return adcValue2 ;
}
