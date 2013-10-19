/*
 * udp.c
 *
 * Created: 3/27/2013 8:23:20 PM
 *  Author: ben
 */ 
#include <avr/io.h>
#include "udp.h"
#include "net.h"
#include "eth.h"
#include "ipv4.h"
#include "in.h"
//#include "../enc28j60.h"

//Offsets into the packet representing where the data for each layer starts
#define IP_START		sizeof(ETH_Data_t)
#define UDP_START		sizeof(ETH_Data_t)+sizeof(IPv4_Data_t)


//Documentation in header file
void udp_makeHeader(uint8_t *pbuf, uint16_t senderPort, uint16_t targetPort)
{
	UDP_Data d = (UDP_Data)(pbuf + UDP_START);
	
	d->senderPort = htons(senderPort);
	d->targetPort = htons(targetPort);
	d->length = 0x0000;//MUST BE UPDATED!
	
	ns.plength += sizeof(UDP_Data_t);
}

void udp_setDataLength(uint8_t *pbuf, uint16_t dataLength)
{
	UDP_Data d = (UDP_Data)(pbuf + UDP_START);
	d->length = htons(dataLength + sizeof(UDP_Data_t));//UDP Data length includes header
}

void udp_computeChecksum(uint8_t *pbuf)
{
	UDP_Data udp = (UDP_Data)(pbuf + UDP_START);
	IPv4_Data ip = (IPv4_Data)(pbuf + IP_START);
	uint16_t *p;
	uint16_t i, count, udpLen;
	udpLen = ntohs(udp->length);
	
	//Start with temporary 32-bit checksum at zero
	uint32_t checksum = 0;
	
	//Make sure the checksum in pbuf is zero
	udp->checksum = 0x0000;
	
	//Add IP source and target data
	//p is a pointer to arrays of 16-bit uints starting at the senderIP in the IP packet
	p = (uint16_t*)ip->senderIP;
	for(i = 0; i < 4; i++)
		checksum += ntohs(p[i]);
		
	//Add Protocol and the length of the packet
	checksum += ip->protocol + ntohs(udp->length);
	
	//Make a pointer to the udp section
	p = (uint16_t*)(pbuf + UDP_START);
	
	//Sum all data within the udp packet including udp header
	//uint16_t temp;
	//uint16_t *tp;
	count = udpLen/2;
	for(i = 0; i < count; i++)
		checksum += ntohs(p[i]);	
		
	if(udpLen & 0x0001)
		checksum += (ntohs(p[udpLen/2] & 0x00FF));
		
	//Add carry bits back to checksum
	while(checksum & 0xFFFF0000)//While checksum is larger than 16 bits
		checksum = (checksum >> 16) + (checksum & 0x0000FFFF);
		
	//Save 1's complement of the 16-bit checksum	
	udp->checksum = htons(~((uint16_t)checksum));
}

/*
void udp_computeChecksum(uint8_t *pbuf)
{
	UDP_Data udp = (UDP_Data)(pbuf + UDP_START);
	IPv4_Data ip = (IPv4_Data)(pbuf + IP_START);
	uint16_t *p;
	uint16_t i, count, udpLen;
	udpLen = ntohs(udp->length);
	
	//Start with temporary 32-bit checksum at zero
	uint32_t checksum = 0;
	
	//Make sure the checksum in pbuf is zero
	udp->checksum = 0x0000;
	
	//Add IP source and target data
	//p is a pointer to arrays of 16-bit uints starting at the senderIP in the IP packet
	p = (uint16_t*)ip->senderIP;
	for(i = 0; i < 4; i++)
		checksum += p[i];
		
	//Add Protocol and the length of the packet
	checksum += (((uint16_t)ip->protocol) << 8) + udp->length;
	
	//Make a pointer to the udp section
	p = (uint16_t*)(pbuf + UDP_START);
	
	//Sum all data within the udp packet including udp header
	//uint16_t temp;
	//uint16_t *tp;
	count = udpLen/2;
	for(i = 0; i < count; i++)
		checksum += p[i];	
		
	if(udpLen & 0x0001)
		checksum += (p[udpLen/2] & 0x00FF);
		
	//Add carry bits back to checksum
	while(checksum & 0xFFFF0000)//While checksum is larger than 16 bits
		checksum = (checksum >> 16) + (checksum & 0x0000FFFF);
		
	//Save 1's complement of the 16-bit checksum	
	udp->checksum = ~((uint16_t)checksum);
}
*/