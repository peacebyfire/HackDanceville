/*
 * temp.c
 *
 * Created: 7/24/2012 12:46:07 PM
 *  Author: ben
 */ 

#include "temp.h"
#include <avr/io.h>

#define Tc		0.0195 // 19.5 mV/C
#define V0c		0.400 //  400 mV
#define Vref	3.300 //  3.3 V

//ADC CH 6
void init_temp()
{
	
	//AVCC is ref with cap at AREF pin, channel ADC6 selected, right adjusted
	ADMUX = 0b01000110;
	
	//Enable ADC
	//Target freq 100 kHz => prescaler F_CPU / 128 (min freq)
	//Free running mode
	ADCSRA = 0xA7;
	
	//Free running mode will constantly update the result registers
	
	//Start conversion
	ADCSRA |= 0x40;
	
}

//Returns the temperature sensor reading in deg C/512
uint16_t getTemp()
{
	uint16_t result = 0;
	
	//Get the results from the latest conversion
	result |= ADCL;
	result |= (ADCH << 8);
	
	return (result*(32*Vref) - V0c*32768)/(32768*Tc);
}

uint16_t getTempRaw()
{
	uint16_t result = 0;

	result |= ADCL;
	result |= (ADCH << 8);
	
	return result;
}	
