// File:   ThermistorConv.c
// Author: H. Martin
// Date: 31/3/22
// Version 1.0

#include <xc.h>

//Define Lookup Table (LUT) for thermistor response when biased with 4.7 k resistor

const unsigned int ThermValueLUT[] = {869,860,852,844,837,829,821,813,804,795,791,782,772,763,753,743,732,722,711,700,687,674,664,654,644,633,622,612,601,590,579,573,562,551,540,529,517,506,495,484,473,462,450,439,428,418,407,396,385,372,372,360,352,343,334,326,317,309,301,293,285,277,270,262,255,248,241,234,227,220,214,210,204,198,192,186,177,177,167,163,158,154,149,145,141,137,133,129,125,121,118,116,112,109,106,103,99,96,93,90,86,85,83,80,78,76,74,72,70,68,66,65,63,61,60,58,56,55,53,52,50};
const signed char TempLUT[] = {-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110};

//Define array size for the table 
unsigned char temparraysize = 121;



signed char GetThermistorTemp (unsigned int ADCValue)
{
    unsigned char i;
    
    // Do linear search on look up table (LUT) against current ADC value
    for(i=0; i<temparraysize; i++)
    {
        if (ThermValueLUT[i]<ADCValue) return TempLUT[i];
    }
    return -99;      // Returns code -99 if the ADC is out of bounds
}