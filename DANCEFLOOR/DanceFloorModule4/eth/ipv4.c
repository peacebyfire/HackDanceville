/*
 * ipv4.c
 *
 * Created: 3/27/2013 5:36:01 PM
 *  Author: ben
 */ 

#include "ipv4.h"
#include "net.h"
#include "eth.h"
#include "in.h"
//#include "../enc28j60.h"

//Offsets into the packet representing where the data for each layer starts
#define IP_START		sizeof(ETH_Data_t)

uint8_t zeroIP[] = {0,0,0,0};
uint8_t bcastIP[] = {255,255,255,255};

//Documentation in header file
void ip_makeHeader(uint8_t *pbuf, IP_PROTOCOL protocol, uint8_t *senderIP, uint8_t *targetIP)
{
	IPv4_Data d = (IPv4_Data)(pbuf + IP_START);
	
	d->IPver4_Length4 = IPVER_HDRLEN;
	//d->IPversion = 0x5;//
	//d->hdrLength = 0x4;//Swapped, it writes it backwards for some reason
	d->typeOfService = 0x00;
	d->totalLength = 0x0000; //THIS MUST BE UPDATED! (data size + 20)
	d->identification = 0x0000; //Since we don't want fragmentation this will be zero unless otherwise.
	d->flags3_offset13 = 0x0000;//No flags
	d->ttl = 128;
	d->protocol = protocol;
	for(uint8_t i = 0; i < 4; i++)
	{
		d->senderIP[i] = senderIP[i]; //Sender IP
		d->targetIP[i] = targetIP[i]; //Target IP
	}
	
	d->checksum = 0x0000; //MUST BE UPDATED!
	
	ns.plength += sizeof(IPv4_Data_t);
}

//Documentation in header file
void ip_setDataLength(uint8_t *pbuf, uint16_t dataLength)
{
	IPv4_Data d = (IPv4_Data)(pbuf + IP_START);
	
	d->totalLength = htons(dataLength + sizeof(IPv4_Data_t));
}

//Documentation in header file
void ip_computeChecksum(uint8_t *pbuf)
{
	IPv4_Data d = (IPv4_Data)(pbuf + IP_START);
	uint16_t *p = (uint16_t*)(pbuf + IP_START);
	
	//Temporary 32-bit checksum starts at zero
	uint32_t checksum = 0;
	
	//Make sure checksum in pbuf is zero
	d->checksum = 0x0000;	
	
	//Interestingly, a little endian summation works correctly on big-endian data
	for(uint8_t i = 0; i < sizeof(IPv4_Data_t)/2; i++)
		checksum += p[i];
	
	//Add carry bits back to checksum
	while(checksum & 0xFFFF0000)//While checksum is larger than 16 bits
		checksum = (checksum >> 16) + (checksum & 0x0000FFFF);
	
	//Save checksum as 1's complement 16-bit value	
	d->checksum = ~((uint16_t)checksum);
	
}