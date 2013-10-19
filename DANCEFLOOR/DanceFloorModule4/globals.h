
#ifndef GLOBALS_H
#define GLOBALS_H

#include <avr/eeprom.h>


//*************************************************
//				CONGIGURATION BEGIN
//*************************************************

#define DF_TILENUM	4		//The tile number of this tile

#define DF_PORT		21337	//The network port to send/recv from

#define MAX_TEMP_C	65		//The maximum temp in C before shutdown conditions (Approx 150 F) (NOT YET IMPLEMENTED!)

//#define SETIP	//If defined, IP addresses will be requested based on 192.168.1.(IP_BASE + tileNum)
//#define IP_BASE	150

//*************************************************
//				CONGIGURATION END
//*************************************************



//#define F_CPU 6250000 //6.25 MHz
#define F_CPU 12500000 //12.5 MHz

#define true	1
#define false	0

extern uint8_t tileNum;


#endif