/*
 * eth.c
 *
 * Created: 3/27/2013 5:11:32 PM
 *  Author: ben
 */ 

#include <avr/io.h>
#include "eth.h"
#include "net.h"
#include "in.h"

uint8_t bcastMAC[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

//Documentation in header file
ETH_PTYPE eth_getPacketType(uint8_t *pbuf)
{
	ETH_Data d = (ETH_Data)pbuf;
	return ntohs(d->ptype);
}

//Documentation in header file
void eth_makeHeader(uint8_t *pbuf, uint8_t *srcMAC, uint8_t *destMAC, ETH_PTYPE ptype)
{
	ETH_Data d = (ETH_Data)pbuf;
	
	for(uint8_t i = 0; i < 6; i++)
	{
		d->srcMAC[i] = srcMAC[i];
		d->destMAC[i] = destMAC[i];
	}
	
	d->ptype = htons(ptype);
	
	ns.plength = sizeof(ETH_Data_t);
}