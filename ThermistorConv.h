// File:   ThermistorConv.h
// Author: H. Martin
// Date: 31/3/22
// Version 1.0

// This header file contains the function prototypes for the thermistor look up table

// This function takes the ADC value as the parameter and returns an integer temperature in degrees C
// The function contains stored values for the range -10 to 110 deg C. If the ADC value
// falls out of this range the function will return -99.
// The look up table values are only valid for a TTC05502 in a potential
// divider configuration with a  4.7k bias resistor and a 5V supply.

signed char GetThermistorTemp (unsigned int);
