// Filename:  ADClib882.c 
// Version:  1.0 
// Date:  <Insert current date> 
// Author: <Insert your name> 
// Purpose     : Provides functions to easily use PIC16F882 ADC 
#include <xc.h> 
#define _XTAL_FREQ 4000000  // Required for the __delay_us and __delay_ms macros 
 
/* ADC_Config Configures the ADCON1 register for Fosc/8 and enables the ADC 
 * function once before prior to using any of the LCD display functions 
 * contained in this library. */ 
 
void ADC_initialise(void)  
{ 
    TRISB  |= 0x18;   // Set RB3&4 as inputs as required for ADC lines 
    ANSELH |= 0x0A;   // Set RB4 and RB4 as analogue inputs AN9 and AN11      
    ADCON0 = 0x6C;      // Set ADC to: Channel AN11, conversion clock = Fosc/8 
    ADCON1 = 0x80;      // Set ADC to: Internal Vref, right justified results 
    ADCON0bits.ADON = 1;    // Turn ADC On 
} 
 
/* ADC_Read reads the current analogue reading channel selected. It starts the  
 * conversion by setting the Go/Done bit. Conversion is complete when the bit 
 * is cleared by the MCU so a polling loop is set up detect this. After 
 * conversion the ADRESH and ADCRESL are combined to provide a 10 bit result. 
  86
 */ 
 
unsigned int ADC_read(void) 
{ 
    unsigned int result; 
    __delay_us(5);               // Wait for acquisition time (worst case 5 us) 
    ADCON0bits.GO = 1;            // Set GO Bit to start conversion 
   
    while(ADCON0bits.GO==1);      // Wait for GO bit to clear=conversion complete 
   
    result =(ADRESH<<8)+ADRESL;   // Combine to produce final 10 bit result 
    return(result); 
} 
 
/* ADC_channel_select() selects the current channel for conversion. 
* On the E?block sensor board:  
* Channel 0 (AN0) is connected to the LDR.  
* Channel 1 (AN1) is connected to the potentiometer. 
*/ 
 
void ADC_channel_select(unsigned char channel) 
{ 
    ADCON0bits.CHS=channel;       // Select ADC analogue input (AN9 or AN11) 
}
