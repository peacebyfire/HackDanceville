/*
 * systime.c
 *
 * Created: 7/14/2012 5:37:52 PM
 *  Author: ben
 */ 

#include <avr/interrupt.h>
#include "globals.h" //For F_CPU
#include "systimer.h"






//The number of ms elapsed since system start
TIMER_TYPE ms_elapsed;

//The set values of the timers
TIMER_TYPE timer_values[MAX_TIMERS];

//The status of the timers
int8_t timer_status[MAX_TIMERS];

//uint8_t fractionalCount; //The number of 1/16ms to be added in every 16 counts

void init_timer()
{
	//Clear all
	ms_elapsed = 0;
	for(int i = 0; i < MAX_TIMERS; i++)
	{
		timer_values[i] = 0;
		timer_status[i] = TIMER_UNUSED;
	}		


	//fractionalCount = 16;
	
	//Use TIMER2 as ms counter system timer
	TCCR2A = 0x02;//No compare match output, CTC mode
	TIMSK2 = 0x02;//Enable COMPA interrupt
	
	OCR2A = 195; //1ms at F_CPU/64 where F_CPU = 12.5 MHz
	
	TCCR2B = 0x04;//Use F_CPU/64 => 3125/16 ticks/ms (enables timer)
}

int8_t timer_get()
{
	for(uint8_t i = 0; i < MAX_TIMERS; i++)
		if(timer_status[i] == TIMER_UNUSED)
		{
			timer_status[i] = TIMER_EXPIRED;
			return i;
		}
	//If no timers are free
	return -1;
}

void timer_release(int8_t timer)
{
	timer_status[timer] = TIMER_UNUSED;
}



/*
TIMER_TYPE millis()
{
	return ms_elapsed;
}
*/
void timer_set(uint8_t timer, TIMER_TYPE ms_delay)
{
	timer_status[timer] = TIMER_RUNNING;
	timer_values[timer] = ms_elapsed + ms_delay;	
}

uint8_t timer_check(uint8_t timer)
{
	
	return timer_status[timer];
}


ISR(TIMER2_COMPA_vect)
{
	/*
	fractionalCount--;
	if(fractionalCount == 0)//Account for fractional ms
	{
		fractionalCount = 16;
		ms_elapsed += 2;
	}
	*/
	//else
	//{
		
		for(int i = 0; i < MAX_TIMERS; i++)
		{
			if(timer_status[i] == TIMER_RUNNING)
				if(ms_elapsed == timer_values[i])
					timer_status[i] = TIMER_EXPIRED;
		}
		

		ms_elapsed++;
	//}
}
