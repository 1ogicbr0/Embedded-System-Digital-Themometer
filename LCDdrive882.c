// Author      : Haydn Martin
// Version:    : 3.0
// Date        : 21/11/21
// Filename    : LCDdrive882.c
// Purpose     : Provides function to drive the Matrix LCD display in MPLAB X

// Function prototypes for internal functions

void LCD_write(unsigned char c, unsigned char rs);
void LCD_clear(void);                              // function to clear LCD display and home the cursor

#include <xc.h>
#include <stdlib.h>

// LCD Driver module
// LCD module is connected as follows:
// RB0 - RB3 -> DB4 - DB7   (data lines)
// RB4 -> RS                (control line)
// RB5 -> E                 (control line)

// Define some useful constants
#define RS PORTCbits.RC0               // RS control line is bit 4 of port B
#define E  PORTCbits.RC3               // E control line is bit 5 of port B


#define LCD_INIT 0x33             // command to initialise LCD display
#define FOUR_BIT_MODE 0x32        // command to make the LCD transfer instructions and data in 4 bit chunks rather than 8
#define TWO_LINE_MODE 0x2c        // command to put the LCD in two-line mode
#define MOVE_AFTER_WRITE 0x06     // command to increment cursor position automatically
#define DISPLAY_INIT 0x0c         // command to turn display on, switch off the cursor and stop it blinking

#define WRITE_DELAY 250                // constant used to set write delay time (50 us required)
#define BIT_DELAY 2                    // constant used to set pulse duration (2/3 us required)
#define INST_DELAY 1250                // constant used to det instruction delay (1.53 ms required)

//#define WRITE_DELAY 50                 // constant used to set write delay time (50 us required)
//#define BIT_DELAY 3                    // constant used to set pulse duration (2/3 us required)
//#define INST_DELAY 200                 // constant used to det instruction delay (1.53 ms required)


// Function to initialise LCD module
void LCD_initialise(void)
{
    unsigned int counter;
        // INITIALISE LCD
        LCD_write(LCD_INIT, 0);             // command to initialise LCD display, (RS = 0)
        for (counter = 0; counter < INST_DELAY; counter++); //wait until LCD has completed instruction
        
        // Set LCD for 4-bit mode
        LCD_write(FOUR_BIT_MODE, 0);        // command to set LCD in 4-bit mode, (RS = 0)
        for (counter = 0; counter < INST_DELAY; counter++); //wait until LCD has completed instruction

        // set LCD in up for two lines
        LCD_write(TWO_LINE_MODE, 0);        // command to set LCD for two lines, (RS = 0)
        for (counter = 0; counter < INST_DELAY; counter++); //wait until LCD has completed instruction

        // Select move after write
        LCD_write(MOVE_AFTER_WRITE, 0);     // command to set LCD to increment cursor pos. automatically (RS = 0)
        for (counter = 0; counter < INST_DELAY; counter++); //wait until LCD has completed instruction

        // Configure display & cursor
        LCD_write(DISPLAY_INIT, 0);         // command to configure display, (RS = 0)
        for (counter = 0; counter < INST_DELAY; counter++); //wait until LCD has completed instruction

        LCD_clear();
}



// LCD_write() function is a basic write subroutine that could be an instruction or data
//  Note: PORTB interrupts are not available during execution of this function

void LCD_write(unsigned char c, unsigned char rs)
{
        unsigned char temp, intsON;     // temporary variable to hold ms 4 bits of c
        unsigned char RSmask = 0;       // used to set or reset bit 4 of PORTB
        unsigned char TRISC_BAK;         // Current status of port B data direction register
        //unsigned char RB7_BAK, RB6_BAK;  // 
        unsigned char INTCON_BAK;
        unsigned int counter;            // Used for producing required delay when pulsing line E
        //unsigned char PORTB_BAK;

        // INITIALISATION
        intsON = INTCONbits.GIE;        // Store GIE status and disable interrupts
        INTCONbits.GIE = 0;
        TRISC_BAK = TRISC;              // Get current state of TRISB register
        //RB7_BAK = RB7;      // Store the state of the 2 MSBs on PORTB
        //RB6_BAK = RB6;      // These lines may be used for non-LCD purposes
                            // NB it would be better to deal with this using masks
        
        TRISC = 0x00;                   // Set PORTB bits 0 - 7 as outputs

        if (rs == 1) RSmask = 0x01;        // set up bit mask for RS line

        // set MS 4 bits of c up on RB0 - RB3
        temp = c;
        PORTC = (temp & 0xF0) | RSmask;      // present LS 4 bits on RB0 - RB3 and set/rst RS line

        // pulse E high then low
        // set E high and wait a bit
        E = 1;                                // set E line high
        for (counter = 0; counter < BIT_DELAY; counter++); // pulse delay for E
        // set E low and wait a bit
        E = 0;                                // set E line low
        for (counter = 0; counter < BIT_DELAY; counter++); // pulse delay for E
        
        // set LS 4 bits up on RB0 - RB3
        PORTC = (c << 4) | RSmask;     // shift MS 4 bits into LS 4 bits, set/rst RS and present on PORTB

        // pulse E high then low
        // set E high and wait a bit
        E = 1;                                // set E line high
        for (counter = 0; counter < BIT_DELAY; counter++); // pulse delay for E
        // set E low and wait a bit
        E = 0;                                // set E line low
        for (counter = 0; counter < BIT_DELAY; counter++); // pulse delay for E
        
        //PORTB = 0x10;              // set E high again  to avoid annoying LED flicker
        
        //RB7 = RB7_BAK;             // Restore the state of the 2 MSBs on PORTB
        //RB6 = RB6_BAK;
        
        TRISC = TRISC_BAK;                // Restore state of TRISB register
        //temp = PORTC;                     // Dummy read of PORTB to avoid retriggering RBIF
        
        //INTCONbits.RBIF=0;                // Clear RBIF interrupt flag to avoid false trigger on exit
        //INTCONbits.INTF=0;                // Clear RBIF interrupt flag to avoid false trigger on exit
        
        if(intsON) GIE = 1;
}



//putch(c) function is character output routine
void LCD_putch(unsigned char c)
{
        unsigned int counter;
        LCD_write(c, 1);                // call basic LCD write routine with RS high (send data)
        for (counter = 0; counter < WRITE_DELAY; counter++); //wait until LCD has completed write operation
}

//LCD_clear() function is an instruction to clear all data from LCD display and return cursor to beginning of display
void LCD_clear(void)
{
        unsigned int counter;
        LCD_write(1, 0);                // call basic write function with instruction 1, (clear display) and RS 0, (write instruction)
        for (counter = 0; counter < INST_DELAY; counter++); //wait until LCD has completed instruction
        LCD_write(2, 0);                // call basic write function with instruction 2, (cursor home) and RS = 0, (write instruction)
        for (counter = 0; counter < INST_DELAY; counter++); //wait until LCD has completed instruction
}

// Function to write null terminated string to LCD display
void LCD_puts(unsigned char *s)
{
        unsigned char c = 255;
        unsigned char i = 0;
        while ((c > 0) && (i < 16)) //16 is the maximum length of the string
        {
          c = s[i];
          if (c != 0) LCD_putch(c);
          i++;
        }
}

// Function to write null terminated string to LCD display
void LCD_putsc(const unsigned char *s)
{
        unsigned char c = 255;
        unsigned char i = 0;
        while ((c > 0) && (i < 16)) //16 is the maximum length of the string
        {
          c = s[i];
          if (c != 0) LCD_putch(c);
          i++;
        }
}

void LCD_cursor (unsigned char x, unsigned char y )
{
    if ( y==0 )
    {
        /* position for line 0     */
        y=0x80 ;
    }
    else
    {
        /* position for line 1     */
        y=0xc0 ;
    }

    LCD_write( y+x, 0) ;
}

void LCD_cursor_on ( void )       // Turn cursor on
{
    LCD_write(0x0f, 0);
}

void LCD_cursor_off ( void )      // Turn cursor off
{
    LCD_write (0x0c, 0);
}

void LCD_display_value (long value)
{
    char buffer[12];          // Buffer for storing ltoa() conversions)
                              // Max 10 digits + null and sign
    ltoa(buffer, value, 10);  // Do conversion using stdlib function itoa()
    LCD_puts(buffer);         // Output to LCD display
}


//void LCD_display_float (float value)
//{
//   char buffer[12];         // Buffer for storing ltoa() conversions)
//                            // Max 10 digits + null and sign
//   char output[7];          // Assign output char array sized for 4 sigfigs ,sign and decimal place
//   unsigned char str_len=0, isneg=0, i, j=1, pos=0;
//   signed char offset;
//   
//   
//   if (value<0) isneg=1;            // Flag for negative values
//
//  for(i=0; i<dplaces;i++)           // Scale float to make all digits of interest
//        value=value*10;             // as integers. The fractional part is then
//                                    // truncated following rounding
//   if(isneg)                        // Round integer part from fractional
//   {
//       value -= 0.5;                // If neg round accordingly and then get abs value
//       value = value * -1;          // and add minus sign
//
//   }
//   else
//       value += 0.5;                // If positive just round accordingly
//
//  ltoa(buffer, (long)value, 10);    // Convert integer part by truncation of fractional
//
//  while(buffer[str_len])
//    str_len++;                      // Get string length
//
//   if(str_len > 4)  // Limit output to 4 sig figs to avoid rounding corruption
//   {
//        LCD_putsc("ERR");     // This is due to lack of headroom for moving the decimal place above
//        return;
//   }
//
//   offset = str_len - dplaces;    // Offset location for decimal place in output
//
//   if(isneg)      // Add negative sign if non-zero result
//   {
//       for(i=0;i<str_len;i++)   // Only add negative signs if there are non-zero values present
//       {
//           if(buffer[i] != '0') // Search string to establish this
//           {
//               output[pos]='-';
//               pos++;
//               break;
//           }
//       }
//   }
//
//   if(offset>0)                       // Is there an integer part?
//   {
//        for(i=0;i<offset;i++)         // Create integer part of the output string
//        {                        // and add decimal point if needed
//            output[pos]=buffer[i];
//            pos++;
//        }
//    }
//   else                             // If no integer part write zero
//   {
//       output[pos] = '0';
//       pos++;
//   }
//
//   if(dplaces)            // Do fractional digit(s) need appending?
//   {
//       output[pos] = '.';   // Write decimal point
//       pos++;
//       for(offset; offset<0;offset++)   // Write any leading zeroes
//       {                                // offset is negative in this case
//           output[pos]='0';
//           pos++;
//           
//       }
//        for(i=offset; i<str_len;i++)  // Step through and append fractional digits
//        {
//            output[pos]=buffer[i];
//            pos++;
//        }
//   }
//
//   output[pos]=0x00;    // Append null character (saves initialising output array)
//   LCD_puts(output);
//}
