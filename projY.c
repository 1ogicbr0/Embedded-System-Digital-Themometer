// Filename: DigitalThromometer
// Version: 1.0
// Date:   1/05/2022
// Author: Maham Gulzar
//

#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits  
#pragma config WDTE = OFF       // Watchdog Timer Enable bit  
#pragma config PWRTE = OFF      // Power?up Timer Enable bit  
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit  
#pragma config CP = OFF         // Code Protection bit  
#pragma config CPD = OFF        // Data Code Protection bit
#pragma config BOREN = OFF      // Brown Out Reset Selection bits
#pragma config IESO = OFF       // Internal External Switchover bit
#pragma config FCMEN = OFF      // Fail?Safe Clock Monitor Enabled bit
#pragma config LVP = OFF        // Low Voltage Programming Enable bit
#pragma config BOR4V = BOR40V   // Brown?out Reset Selection bit
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits
#include <xc.h>             // Required by compiler, PIC specific definitions
#include "LCDdrive882.h"    // Header file needed to access to LCD custom library
#include "ADClib882.h"
#include "ThermistorConv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define _XTAL_FREQ 1000  // MCU clock speed ? required for delay macros
#define MAX 10000

//Declaration of functions and global variables + constants added
void __interrupt() my_ISR(void);
void mv_Right(int);
void compareTemp();
void displaySetup();
void convrtDegree();
int ConvCtoF(int);
void flip_deg();
void reset();
char Degree = 'C'; //or F
int temp;
int tempMin = 200;
int tempMax = -50;
int count_Interrupt=0;
const char myString1[] = {"Temp="};
const  char myString2[] = {"Min="};
const  char myString3[] = {"Max="};


            
void main(void)
{

    //GPIO setup
    ANSEL = 0x00;     // Set all pins to digital
    ANSELH = 0x00;
    TRISC = 0x00;   // Set all PORTC to outputs (for LCD)
    PORTC = 0x00;
    TRISB0 = 0x01;               // Set RB0 (SW0) as input
    TRISB1 = 0x01;               // Set RB1 (SW1) as input
    PORTB = 0x00;
    //Timer0 setup
    OPTION_REGbits.T0CS = 0;    // Set clock source to internal (timer mode)
    OPTION_REGbits.PSA = 0;     // Set prescaler to Timer 0
    OPTION_REGbits.PS = 3;      // Set prescaler bits to 011 for 1:16
    //Interrupt setup
    INTCONbits.RBIE = 1;        // Enable Timer0 overflow interrupt
    INTCONbits.RBIF = 0;        // Clear Timer0 overflow interrupt
    IOCBbits.IOCB = 1;          // Enable RBIE on RB0 only
    IOCBbits.IOCB1 = 1; 
    INTCONbits.GIE = 1;         // Enable interrupts
     
    //Our Setup
    LCD_initialise();
    ADC_initialise();
    temp = GetThermistorTemp(ADC_read());
    int refresh = 0;
    int tempCheck;
    
    while(1) //LOOP Started
    {
    refresh = refresh + 1;
    if(refresh%2==0)
    {
        convrtDegree();
    }
    if(count_Interrupt==1)   //For Clearing LCD everyTime interrupt is pressed
    {
        LCD_clear();
        count_Interrupt=0;
    }
    compareTemp();
    displaySetup();
    __delay_ms(1000);
    if(refresh%8==0)
    {
        LCD_clear();
    }

}
}

void reset()
{
    tempMin=temp;
    tempMax=temp;
}
int ConvCtoF(int tempC)
{
   int tempF = ((tempC*2))+32;
   return tempF;
}

void mv_Right(int a)
{
    if(a<10)
    {
    LCD_cursor(5,0);
    LCD_putch(' ');
    }      
}
void flip_deg()
{
    tempMin=200; //reseting Min for comparison
    tempMax=-50; //resetting Max for comparison
    if (Degree=='C')
            Degree='F';
    else
        Degree='C';
}
void compareTemp() //Comparison Min/Max
{
    if(tempMin>temp)
     {
        tempMin=temp;
     }
    if(tempMax<temp)
     {
       tempMax=temp;
     }
}

void displaySetup() //LCD Display here
{
    LCD_cursor(0,0);
    LCD_puts(myString1);
    LCD_cursor(5,0);
    mv_Right(temp);
    ADC_channel_select(11);
    LCD_display_value(temp);
    if(temp!=-99)
    {
    LCD_putch('.');
    LCD_putch('0');
    }

    LCD_cursor(10,0);
    LCD_puts(" deg ");
    LCD_cursor(15,0);
    LCD_putch(Degree);
    LCD_cursor(0,1);
    LCD_puts(myString2);
    LCD_display_value(tempMin);
    LCD_cursor(9,1);
    LCD_puts(myString3);
    LCD_cursor(13,1);
    LCD_display_value(tempMax);
}

void convrtDegree()
{
    int tempCheck;
            if(Degree=='F')
        {
            tempCheck = GetThermistorTemp(ADC_read());
            if(tempCheck!=-99)
               temp = ConvCtoF(tempCheck);
            else
                temp = GetThermistorTemp(ADC_read());
        }
        else
        {
            temp = GetThermistorTemp(ADC_read());
        }
}

//Interrupt Function
 void __interrupt() my_ISR(void)
{
  if(INTCONbits.RBIF)
 {
    if(PORTBbits.RB0)   //FOR RB0 change between C and F
        {
            count_Interrupt++;
            flip_deg();
        }
    PORTBbits.RB0 = 0;  //RB0 back to normal
        
    if(PORTBbits.RB1)   //FOR RB1 to reset Min and Max temperature
            count_Interrupt++;
            reset();
    PORTBbits.RB1 = 0;   //RB1 back to normal
    INTCONbits.RBIF = 0;// Clears interrupt flag back to normal
   }
}