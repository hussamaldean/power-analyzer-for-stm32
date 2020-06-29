#include "VI.h"
uint16_t startV;
#define ADC_BITS 12
#define ADC_COUNTS  4095

int sampleV;                        
int sampleI;
int SupplyVoltage=3300;
unsigned int crossCount = 0;                             
unsigned int numberOfSamples = 0;                        
unsigned long start;
int sampleV;                        //sample_ holds the raw analog read value
int sampleI;
double ICAL;
double lastFilteredV,filteredV;          //Filtered_ is the raw analog value minus the DC offset
double filteredI;
double offsetV;                         //Low-pass filter output
double offsetI;                        //Low-pass filter output
double phaseShiftedV;                             //Holds the calibrated phase shifted voltage.
double sqV,sumV,sqI,sumI,instP,sumP;              //sq = squared, sum = Sum, inst = instantaneous
bool lastVCross, checkVCross; 
double realPower,apparentPower,powerFactor,Vrms,Irms;

void calcVI( uint16_t crossings,  uint16_t timeout, uint16_t PHASECAL ,double Vcal,double Ical)
{
ICAL=Ical;
SupplyVoltage=3300;
crossCount = 0;                             
numberOfSamples = 0;                        
start = millis(); 
while(1)                                   
  {
    startV = readADCVolt();                   
    if ((startV < (ADC_COUNTS*0.55)) && (startV > (ADC_COUNTS*0.45))) break;  
    if ((millis()-start)>timeout) break;
  }
	
	start = millis();

  while ((crossCount < crossings) && ((millis()-start)<timeout))
  {
    numberOfSamples++;                       
    lastFilteredV = filteredV;              
    sampleV = readADCVolt();                 //Read in raw voltage signal
		delayuS(1);
    sampleI = readADCCurrent();                 //Read in raw current signal
    offsetV = offsetV + ((sampleV-offsetV)/ADC_COUNTS);
    filteredV = sampleV - offsetV;
    offsetI = offsetI + ((sampleI-offsetI)/ADC_COUNTS);
    filteredI = sampleI - offsetI;

    sqV= filteredV * filteredV;                 //1) square voltage values
    sumV += sqV;                                //2) sum


    sqI = filteredI * filteredI;                //1) square current values
    sumI += sqI;                                //2) sum


    phaseShiftedV = lastFilteredV + PHASECAL * (filteredV - lastFilteredV);

       instP = phaseShiftedV * filteredI;          //Instantaneous Power
    sumP +=instP;                               //Sum


    lastVCross = checkVCross;
    if (sampleV > startV) checkVCross = true;
                     else checkVCross = false;
    if (numberOfSamples==1) lastVCross = checkVCross;

    if (lastVCross != checkVCross) crossCount++;
  }



  double V_RATIO = Vcal *((SupplyVoltage/1000.0) / (ADC_COUNTS));
  Vrms = V_RATIO * sqrt(sumV / numberOfSamples);

  double I_RATIO = ICAL *((SupplyVoltage/1000.0) / (ADC_COUNTS));
  Irms = I_RATIO * sqrt(sumI / numberOfSamples);

  realPower = V_RATIO * I_RATIO * sumP / numberOfSamples;
  apparentPower = Vrms * Irms;
  powerFactor=realPower / apparentPower;


 sumV = 0;
 sumI = 0;
 sumP = 0;

}

double getVrms(void)
{
return	Vrms;
}
double getIrms(void)
{
return Irms;
}

double getRealPower(void)
{
return -1*realPower;
}

double getapparentPower(void)
{
return apparentPower;
}

double getpowerFactor(void)
{
return powerFactor;
}


double calcIrms(uint16_t inPinI, unsigned int Number_of_Samples)
{


    int SupplyVoltage=3300;
 


  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    sampleI = readADCCurrent();

    // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
    //  then subtract this - signal is now centered on 0 counts.
    offsetI = (offsetI + (sampleI-offsetI)/4096);
    filteredI = sampleI - offsetI;

    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum
    sumI += sqI;
  }

  double I_RATIO = ICAL *((SupplyVoltage/1000.0) / (ADC_COUNTS));
  Irms = I_RATIO * sqrt(sumI / Number_of_Samples);

  //Reset accumulators
  sumI = 0;
  //--------------------------------------------------------------------------------------

  return Irms;
}


