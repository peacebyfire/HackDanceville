/*
 * arp_ip_udp.h
 *
 * Created: 3/27/2013 2:19:00 PM
 *  Author: ben
 */ 


#ifndef ARP_H_
#define ARP_H_

#include <avr/io.h>

/**
 * ARP Packet Structure.
 */
typedef struct 
{
	uint16_t htype;			/** Hardware Type.				*/
	uint16_t ptype;			/** Protocol Type.				*/
	uint8_t  hlen;			/** Hardware Address Length.	*/
	uint8_t  plen;			/** Protocol Address Length.	*/
	uint16_t oper;			/** Operation.					*/
	uint8_t  senderMAC[6];	/** Sender MAC Address			*/
	uint8_t  senderIP[4];   /** Sender IP Address			*/
	uint8_t  targetMAC[6];	/** Target MAC Address			*/
	uint8_t  targetIP[4];	/** Target IP Address			*/
} ARP_Data_t;

/**
 * ARP Packet Overlay.
 */
typedef ARP_Data_t *ARP_Data;


#define ARP_HTYPE_ETH	1		//Hardware type is Ethernet
#define ARP_PTYPE_IPV4	0x0800	//Protocol type is IPv4
#define ARP_HLEN		6		//6 byte length for MAC Address
#define ARP_PLEN		4		//4 byte length for IP Address

#define ARP_OP_REQUEST	1		//ARP Request
#define ARP_OP_REPLY	2		//ARP Reply

//DONT FORGET ABOUT ENDIANNESS!!


/*! \fn void arp_sendRequest()
 *  \brief Sends an arp request for the MAC address corresponding to the given IP address.
 *  \param ip The IP address corresponding to the MAC address we would like to find.
 */
void arp_sendRequest(uint8_t *ip);


/*! \fn void arp_handlePacket()
 *  \brief Handles a received arp packet. 
 *  If the packet received is a request for this system, a reply is sent. Otherwise, 
 *  the packet is dropped.
 */
void arp_handlePacket();


#endif /* ARP_H_ */