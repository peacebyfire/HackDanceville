/*
 * icmp.c
 *
 * Created: 3/28/2013 3:01:51 PM
 *  Author: ben
 */ 
#include "icmp.h"
#include "eth.h"
#include "ipv4.h"
#include "net.h"
#include "in.h"
#include "enc28j60.h"

void icmp_sendPingReply()
{
	//Temp data
	uint8_t destMAC[6];
	uint8_t destIP[4];
	
	ETH_Data ed  = (ETH_Data)(ns.pbuf);
	IPv4_Data id = (IPv4_Data)(ns.pbuf + sizeof(ETH_Data_t));
	ICMP_Data d  = (ICMP_Data)(ns.pbuf + sizeof(ETH_Data_t) + sizeof(IPv4_Data_t));
	
	for(uint8_t i = 0; i < 6; i++)
		destMAC[i] = ed->srcMAC[i];
	
	for(uint8_t i = 0; i < 4; i++)
		destIP[i] = id->senderIP[i];
		
	for(uint8_t i = 0; i < 6; i++)
	{
		ed->srcMAC[i] = ns.MAC[i];
		ed->destMAC[i] = destMAC[i];
	}
	
	for(uint8_t i = 0; i < 4; i++)
	{
		id->senderIP[i] = ns.IP[i];
		id->targetIP[i] = destIP[i];
	}
	
	d->type = PING_REPLY;
	
	icmp_computeChecksum();
	
	ip_computeChecksum(ns.pbuf);
	
	enc28j60PacketSend(ns.pbuf, ns.plength);
	
}

//Documentation in header file
void icmp_handlePacket()
{
	ICMP_Data d = (ICMP_Data)(ns.pbuf + sizeof(ETH_Data_t) + sizeof(IPv4_Data_t));
	
	switch(d->type)
	{
		case PING_REQUEST:
			//Send reply
			icmp_sendPingReply();		
		break;
	}
}

void icmp_computeChecksum()
{
	IPv4_Data id = (IPv4_Data)(ns.pbuf + sizeof(ETH_Data_t));
	ICMP_Data d  = (ICMP_Data)(ns.pbuf + sizeof(ETH_Data_t) + sizeof(IPv4_Data_t));
	
	uint8_t *p = (uint8_t *)(ns.pbuf + sizeof(ETH_Data_t) + sizeof(IPv4_Data_t));
	uint32_t checksum = 0x0000;
	
	//Start with zero checksum
	d->checksum = 0x0000;
	
	//Compute length of the ICMP payload
	uint16_t length = id->totalLength - sizeof(IPv4_Data_t);
	
	//Sum all data as 16-bit words
	for(uint16_t i = 0; i < (length&1); i+=2)
		checksum += p[i]*256 + p[i+1];
	
	if(length&1)
		checksum += p[length-1]*256;//+ 0x00	
	
	//Add carry bits back to checksum
	while(checksum & 0xFFFF0000)//While checksum is larger than 16 bits
		checksum += (checksum >> 16);
	
	//Save checksum as 16-bit value
	d->checksum = (uint16_t)checksum;
	
	//Complete checksum by computing 1's complement
	d->checksum = htons(~d->checksum);
	
}