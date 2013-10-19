/*
 * net.h
 *
 * Created: 3/27/2013 3:56:34 PM
 *  Author: ben
 */ 


#ifndef NET_H_
#define NET_H_

#include <avr/io.h>

/** 
 *  The state of the network system.
 */
typedef struct  
{
	uint8_t MAC[6]; /** The MAC Address of the network interface */
	uint8_t IP[4];  /** The IP  Address of the network interface */
	uint8_t ipAcquired;
	
	uint8_t hostMAC[6]; /** The MAC Address of the host computer */
	uint8_t hostIP[4];  /** The IP  Address of the host computer */
	uint8_t hostFound;  /** Flag to denote that a host computer has been found */
	
	//uint8_t netmask[4]; /** The network mask of the network interface */
	//uint8_t gatewayIP[4]; /** The IP Address of the network's gateway */
	uint8_t dhcpServerIP[4]; /** The IP Address of the network's DHCP server */
	
	uint8_t *pbuf; /** The packet buffer to use when sending/receiving packets. */
	uint16_t pbuf_size; /** The size of pbuf. */
	uint16_t plength; /** The total length of the packet in the buffer pbuf (not the size of pbuf). */
}NET_State;

extern NET_State ns;


void init_net(uint8_t *pbuf, uint16_t pbuf_size);






#endif /* NET_H_ */