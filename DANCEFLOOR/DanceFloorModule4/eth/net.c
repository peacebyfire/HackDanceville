/*
 * net.c
 *
 * Created: 3/27/2013 5:31:56 PM
 *  Author: ben
 */ 

#include <avr/eeprom.h>
#include "net.h"
#include "../globals.h"

NET_State ns;

//                           T    U    X    D    F    0
uint8_t EEMEM EEmac[6] = {0x54,0x55,0x58,0x44,0x46,0x30 + DF_TILENUM};

void init_net(uint8_t *pbuf, uint16_t pbuf_size)
{
	//Initialize all fields to zero
	uint8_t *ptr = (uint8_t *)&ns;
	for(uint8_t i = 0; i < sizeof(NET_State); i++)
	{
		ptr[i] = 0x00;
	}	

	ns.pbuf = pbuf;
	ns.pbuf_size = pbuf_size;
	
	//Initialize MAC Address from EEPROM
	eeprom_read_block(&ns.MAC, &EEmac, 6);	
}