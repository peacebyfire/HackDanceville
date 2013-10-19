/*
 * arp.c
 *
 * Created: 3/28/2013 1:44:48 PM
 *  Author: ben
 */ 

#include "arp.h"
#include "eth.h"
#include "net.h"
#include "in.h"
#include "enc28j60.h"

/*! \fn void arp_sendReply()
 *  \brief Sends an arp reply with the IP and MAC of this system.
 *  The request packet must be in the buffer when this function is called.
 *  This is only used to fulfill requests for this system's MAC.
 */
void arp_sendReply();
ARP_Data ad;

//Documentation in header file
void arp_sendRequest(uint8_t *ip)
{
	eth_makeHeader(ns.pbuf, ns.MAC, bcastMAC, ETH_ARP);	
	
	ad = (ARP_Data)(ns.pbuf + sizeof(ETH_Data_t));
	
	ad->htype = htons(ARP_HTYPE_ETH);
	ad->ptype = htons(ARP_PTYPE_IPV4);
	ad->hlen  = ARP_HLEN;
	ad->plen  = ARP_PLEN;
	ad->oper  = htons(ARP_OP_REQUEST);
	//Fill MAC Addresses
	for(uint8_t i = 0; i < 6; i++)
	{
		ad->senderMAC[i] = ns.MAC[i];
		ad->targetMAC[i] = 0x00;//We want to find this address
	}
	//Fill IP Addresses
	for(uint8_t i = 0; i < 4; i++)
	{
		ad->senderIP[i] = ns.IP[i];
		ad->targetIP[i] = ip[i];
	}		
	//Set packet size
	ns.plength += sizeof(ARP_Data_t);
	//Send packet
	enc28j60PacketSend(ns.pbuf, ns.plength); 	
}

//Documentation in header file
void arp_sendReply()
{
	ad = (ARP_Data)(ns.pbuf + sizeof(ETH_Data_t));
	
	eth_makeHeader(ns.pbuf, ns.MAC, ad->senderMAC, ETH_ARP);	
		
	//The request packet would not have my MAC but would have my IP
	//Copy the sender addresses into the target address for a reply
	//Fill in this system's data for the sender address
		
	ad->htype = htons(ARP_HTYPE_ETH);
	ad->ptype = htons(ARP_PTYPE_IPV4); //0x0800 (remember, little endian)
	ad->hlen  = ARP_HLEN;
	ad->plen  = ARP_PLEN;
	ad->oper  = htons(ARP_OP_REPLY);

	//Fill MAC Address of target
	for(uint8_t i = 0; i < 6; i++)
	{
		//We want to send a reply back to the sender
		ad->targetMAC[i] = ad->senderMAC[i];
	}
	//Fill IP Address of target
	for(uint8_t i = 0; i < 4; i++)
	{
		ad->targetIP[i] = ad->senderIP[i];
	}
	
	//Fill MAC Address of sender (this)
	for(uint8_t i = 0; i < 6; i++)
	{
		ad->senderMAC[i] = ns.MAC[i];
	}
	//Fill IP Address of sender (this)
	for(uint8_t i = 0; i < 4; i++)
	{
		ad->senderIP[i] = ns.IP[i];
	}
	
	//Set packet size
	ns.plength += sizeof(ARP_Data_t);
	//Send packet
	enc28j60PacketSend(ns.pbuf, ns.plength);
}

//Documentation in header file
void arp_handlePacket()
{
	ad = (ARP_Data)(ns.pbuf + sizeof(ETH_Data_t));
	
	//Determine if the arp request is for this machine
	for(uint8_t i = 0; i < 4; i++)
	{
		if(ad->targetIP[i] != ns.IP[i])
			return; //If it's not a match for this machine's IP, return. The packet is not for us
	}
	
	//If it is a match for our IP, send a reply
	arp_sendReply();
}