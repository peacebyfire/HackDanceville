/*
 * idlePattern.c
 *
 * Created: 10/11/2012 10:33:02 PM
 *  Author: ben
 */ 

#include "idlePattern.h"
#include "tlc5941.h"
#include <stdlib.h> //for rand()


void idlePattern0_ComputeFrame()
{
	int COLORS = 8;
	
	int i = rand() % 64;
	
	setValue(i,rand() % 3,(rand() % COLORS) * 4096/COLORS);
	//setValue(i,GREEN,(rand() % COLORS) * 4096/20);
	//setValue(i,BLUE,(rand() % COLORS) * 4096/20);
	//setValue(i,RED,(int)((double)rand() / ((double)RAND_MAX + 1) * 4096) );
	//setValue(i,GREEN,(int)((double)rand() / ((double)RAND_MAX + 1) * 4096) );
	//setValue(i,BLUE,(int)((double)rand() / ((double)RAND_MAX + 1) * 4096) );
	
	//Rand % zero probability
	
	if((rand() % 1000) < 800)
	{
		i = rand() % 64;
		setValue(i,RED,0);
		setValue(i,GREEN,0);
		setValue(i,BLUE,0);
	}
}

volatile uint8_t p2_j = 0;
enum Color_t color = RED;
void idlePattern1_ComputeFrame()
{
	setValue(p2_j,RED,0);
	setValue(p2_j,GREEN,0);
	setValue(p2_j,BLUE,0);
		
	setValue(p2_j,color,0xFFF);
	
	p2_j++;
	
	if(p2_j > 63)
	{
		p2_j = 0;
		switch(color)
		{
			case RED:
				color = GREEN;
				break;
			case GREEN:
				color = BLUE;
				break;
			case BLUE:
				color = RED;
				break;
				
			default:
				color = RED;
						
		}
	}
}