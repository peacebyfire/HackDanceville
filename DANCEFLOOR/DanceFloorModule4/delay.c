/*
 * delay.c
 *
 * Created: 7/14/2012 5:03:44 PM
 *  Author: ben
 */ 


#include <avr/io.h>
#include "globals.h"
#include "delay.h"
//#include <util/delay.h>


void delay_ms(uint32_t ms)
{
	ms = ms * 1136;
	while(ms--);
}

void delay_us(uint32_t us)
{
	while(us--);
}

