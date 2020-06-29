#ifndef __VI__H
#define __VI__H

#include "adc.h"
#include "delay.h"
#include "stm32f4xx.h" 
#include <stdbool.h>
#include <math.h>


void calcVI(uint16_t crossings,  uint16_t timeout, uint16_t PHASECAL ,double VCAL,double ICAL);

double getVrms(void);
double getIrms(void);
double getRealPower(void);
double getapparentPower(void);
double getpowerFactor(void);
double calcIrms(uint16_t inPinI, unsigned int Number_of_Samples);


#endif


