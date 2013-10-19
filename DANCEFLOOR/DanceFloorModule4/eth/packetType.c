/*
 * packetType.c
 *
 * Created: 3/28/2013 8:00:51 PM
 *  Author: ben
 */ 

#include "packetType.h"
#include "net.h"
#include "eth.h"
#include "arp.h"
#include "ipv4.h"
#include "udp.h"
#include "icmp.h"
#include "dhcp.h"
#include "in.h"
#include "../globals.h"


ETH_Data ed;
IPv4_Data ip;
uint8_t bcastm;//MAC Address is bcast
uint8_t ourm;//MAC address is for us
uint8_t myip;
uint8_t bcastip;
UDP_Data u;
uint16_t targetPort;
DHCP_Data d;

//Documentation in header file
PacketType getPacketType()
{
	//First, make sure the packet type is ethernet and the packet is for this machine
	ed = (ETH_Data)ns.pbuf;
	
	{
		uint16_t ptype = ntohs(ed->ptype);
		//Check that the ethernet protocol is either ARP or IPv4
		if((ptype != ETH_IP) && (ptype != ETH_ARP))
			return Unknown;
			
	
	//Check for bcast MAC Address
	bcastm = 1;
	for(uint8_t i = 0; i < 6; i++)
		if(ed->destMAC[i] != 0xFF)
			bcastm = 0;
			
	//Check if this packet is for our MAC
	ourm = 1;
	for(uint8_t i = 0; i < 6; i++)
		if(ed->destMAC[i] != ns.MAC[i])
			ourm = 0;
	
	//If it isn't a broadcast and isn't for us then return Unknown
	if((bcastm | ourm)==0)
		return Unknown;
	
	//Here, we have an ethernet frame that's either bcast or for us
	
	//Check for ARP
	if(ptype == ETH_ARP)
		return ARP;
	}	
	
	//At this point, there should be an IPv4 header if it is a packet we can handle
	ip = (IPv4_Data)(ns.pbuf + sizeof(ETH_Data_t));
	

	//Check if this packet is for our IP
	myip = 1;
	for(uint8_t i = 0; i < 4; i++)
		if(ip->targetIP[i] != ns.IP[i])
			myip = 0;
	
	//Check if this packet's destination IP is broadcast
	bcastip = 1;
	for(uint8_t i = 0; i < 4; i++)
		if(ip->targetIP[i] != 255)
			bcastip = 0;
	
	//If the destination IP isn't bcast or for us, return Unknown
	//if((myip | bcastip)==0)
	//	return Unknown;
		
	
	//At this point, we have an IP packet that is either bcast or for us
	
	
	//Check for ICMP and our IP
	if((ip->protocol == IP_ICMP) && ((myip | bcastip) > 0))
		return ICMP;
		
		
	//Check for UDP	 type
	if(ip->protocol == IP_UDP)
	{
		u = (UDP_Data)(ns.pbuf + sizeof(ETH_Data_t) + sizeof(IPv4_Data_t));
		targetPort = ntohs(u->targetPort);
		
		//If it's DHCP and for our MAC (or bcast), don't check src and dst IP's
		if(targetPort == DHCP_CLIENT_PORT)
		{
			d = (DHCP_Data)(ns.pbuf + sizeof(ETH_Data_t) + sizeof(IPv4_Data_t) + sizeof(UDP_Data_t));
			//Discard DHCP packets that don't match our transaction ID
			if(d->XID == ns.MAC[5])
				return DHCP;
			else
				return Unknown;
		}		
		//If it's on the DF port, and has myip or bcast
		if((targetPort == DF_PORT) && ((myip | bcastip) > 0))
			return DF;
	}

	//If the packet isn't ICMP or UDP, return Unknown
	return Unknown;	
}