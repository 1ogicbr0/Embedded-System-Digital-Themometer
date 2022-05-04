// Filename:	LCDdrive882.h
// Version:     2.0
// Date:		4/11/21
// Author:		H. Martin
//
// Description: Provides functions to drive a Hitachi HD44680 LCD
// Contains function prototypes for the LCD function library.
// Comments explain the purpose and correct use for each function.

// Function Prototypes declared next

/* LCD_initialise: Send initialisation commands to LCD. You MUST run this 
 * function once before prior to using any of the LCD display functions
 * contained in this library. */
void LCD_initialise(void);                             

/* LCD_clear: Clears the display and homes the cursor to position (0,0) */
void LCD_clear(void);              

/* LCD_putch: Write a single character to the LCD display at the current cursor
 * position  */
void LCD_putch(unsigned char);                     

/* LCD_puts: Write string s to LCD. NB for const string use the LCD_putsc() 
 * function */
void LCD_puts(unsigned char *);                        

/* LCD_putsc: Write const string s to LCD. Use this function to display strings
 * that you have declared as const so they are stored in program memory */
void LCD_putsc(const unsigned char *);                 

/* LCD_cursor: Move cursor to position (row , column). row may assume a value 
 * of 0 or 1 only representing the top and bottom line respectively. the column 
 * parameter may assume any value between 0 (leftmost) and 15 (rightmost) */

void LCD_cursor (unsigned char, unsigned char);   

/* LCD_cursor: Displays a numerical value on the LCD display. The function will
 * calculate and display the correct number of digits up to a maximum of 4. Thus
 * it will correctly display values ranging from 0 to 9999. */

/* LCD_display_value: Displays a numerical value on the LCD display. The 
 * function will calculate and display the correct number of digits for signed 
 * or unsigned values. */

void LCD_display_value (long value);    

/* LCD_display_float: Displays a floating point numerical value on the LCD 
 * This function take the parameter 'value' and displays it to the number of
 * decimal places specified by the parameter 'dplaces'. NOTE: The function
 * is limited to displaying 4 significant figures due to headroom limitations
 * of the 24 bit float type in MPLAB X. Attempting to display more than 4
 * significant figures will lead to the string "ERR" being displayed. */

/* LCD_cursor_on: Turn cursor flash on */
void LCD_cursor_on (void);                              

/* LCD_cursor_off: Turn cursor flash off */
void LCD_cursor_off (void);                             
