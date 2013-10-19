/*
 * dhcp.c
 *
 * Created: 3/27/2013 5:02:38 PM
 *  Author: ben
 */ 

#include "dhcp.h"
#include "net.h"
#include "eth.h"
#include "ipv4.h"
#include "udp.h"
#include "in.h"
#include "enc28j60.h"
#include "packetType.h"
#include "../systimer.h"

//Offsets into the packet representing where the data for each layer starts
#define IP_START		sizeof(ETH_Data_t)
#define UDP_START		sizeof(ETH_Data_t)+sizeof(IPv4_Data_t)
#define DHCP_START		sizeof(ETH_Data_t)+sizeof(IPv4_Data_t)+sizeof(UDP_Data_t)
#define DHCP_OP_START	sizeof(ETH_Data_t)+sizeof(IPv4_Data_t)+sizeof(UDP_Data_t)+sizeof(DHCP_Data_t)


void dhcp_buildPacket(uint8_t *pbuf);

uint8_t offeredIP[4];
//uint8_t dhcpServerIP[4];

IPv4_Data ipd;

//Documentation in header file
void dhcp_sendDiscover()
{
	//dhcpState = DHCP_STATE_DISCOVER;
	//Build ethernet header
	eth_makeHeader(ns.pbuf, ns.MAC, bcastMAC, ETH_IP);
	//Build IP header
	ip_makeHeader(ns.pbuf, IP_UDP, zeroIP, bcastIP);
	//Build UDP header
	udp_makeHeader(ns.pbuf, DHCP_CLIENT_PORT, DHCP_SERVER_PORT);
	//Build DHCP Packet
	dhcp_buildPacket(ns.pbuf);
	
	//Fill in op-codes
	uint8_t *dhcp_op = ns.pbuf+DHCP_OP_START;
		//Set message type to DHCP_DISCOVER
		dhcp_op[0] = DHCP_OP_MESSAGETYPE;
		dhcp_op[1] = 1;//Length
		dhcp_op[2] = DHCP_STATE_DISCOVER;
		
		dhcp_op[3] = DHCP_OP_CLIENTID;
		dhcp_op[4] = 7;//Length
		dhcp_op[5] = 0x01;//Hardware type: ethernet
		for(uint8_t i = 0; i < 6; i++)//Copy MAC
			dhcp_op[i+6] = ns.MAC[i];

		dhcp_op[12] = DHCP_OP_REQIPADDR;
		dhcp_op[13] = 4;//Length
#ifdef SETIP
		dhcp_op[12] = 192;
		dhcp_op[13] = 168;
		dhcp_op[14] = 1;
		dhcp_op[15] = IP_BASE + tileNum;
#else
		dhcp_op[12] = 0;
		dhcp_op[13] = 0;
		dhcp_op[14] = 0;
		dhcp_op[15] = 0;
#endif
			
		dhcp_op[16] = DHCP_OP_HOSTNAME;
		dhcp_op[17] = 3;//Length
		dhcp_op[18] = 'D';
		dhcp_op[19] = 'F';
		dhcp_op[20] = ns.MAC[5];
		
		dhcp_op[21] = DHCP_END;
		
		
	ns.plength += 22;
		
	udp_setDataLength(ns.pbuf, sizeof(DHCP_Data_t)+22);
	udp_computeChecksum(ns.pbuf);
	
	ip_setDataLength(ns.pbuf, sizeof(UDP_Data_t)+sizeof(DHCP_Data_t)+22);
	ip_computeChecksum(ns.pbuf);	
	
	enc28j60PacketSend(ns.pbuf, ns.plength); 
}

//Documentation in header file
void dhcp_sendRequest()
{
	//Get the IP of the DHCP server from the OFFER packet in the buffer
	ipd = (IPv4_Data)(ns.pbuf + IP_START);
	for(uint8_t i = 0; i < 4; i++)
		ns.dhcpServerIP[i] = ipd->senderIP[i];
	
	
	//dhcpState = DHCP_STATE_REQUEST;
	//Build ethernet header
	eth_makeHeader(ns.pbuf, ns.MAC, bcastMAC, ETH_IP);
	//Build IP header
	ip_makeHeader(ns.pbuf, IP_UDP, zeroIP, bcastIP);
	//Build UDP header
	udp_makeHeader(ns.pbuf, DHCP_CLIENT_PORT, DHCP_SERVER_PORT);
	//Build DHCP Packet
	dhcp_buildPacket(ns.pbuf);
	
	//Fill in op-codes
	uint8_t *dhcp_op = ns.pbuf+DHCP_OP_START;
	//Set message type to DHCP_REQUEST
	dhcp_op[0] = DHCP_OP_MESSAGETYPE;
	dhcp_op[1] = 1;
	dhcp_op[2] = DHCP_STATE_REQUEST;
	
	dhcp_op[3] = DHCP_OP_CLIENTID;
	dhcp_op[4] = 7;//Length
	dhcp_op[5] = 0x01;//Hardware type: ethernet
	for(uint8_t i = 0; i < 6; i++)//Copy MAC
		dhcp_op[i+6] = ns.MAC[i];
	
	dhcp_op[12] = DHCP_OP_REQIPADDR;
	dhcp_op[13] = 4;//IP Length
	dhcp_op[14] = offeredIP[0];
	dhcp_op[15] = offeredIP[1];
	dhcp_op[16] = offeredIP[2];
	dhcp_op[17] = offeredIP[3];
	
	dhcp_op[18] = DHCP_OP_SERVERIP;
	dhcp_op[19] = 4;//IP Length
	dhcp_op[20] = ns.dhcpServerIP[0];
	dhcp_op[21] = ns.dhcpServerIP[1];
	dhcp_op[22] = ns.dhcpServerIP[2];
	dhcp_op[23] = ns.dhcpServerIP[3];
	
	dhcp_op[24] = DHCP_OP_HOSTNAME;
	dhcp_op[25] = 3;//Length
	dhcp_op[26] = 'D';
	dhcp_op[27] = 'F';
	dhcp_op[28] = ns.MAC[5];
		
	dhcp_op[29] = DHCP_END;
	ns.plength += 30;
	
	udp_setDataLength(ns.pbuf, sizeof(DHCP_Data_t)+30);
	udp_computeChecksum(ns.pbuf);
	
	ip_setDataLength(ns.pbuf, sizeof(UDP_Data_t)+sizeof(DHCP_Data_t)+30);
	ip_computeChecksum(ns.pbuf);
	
	enc28j60PacketSend(ns.pbuf, ns.plength);
}

//Documentation in header file
void dhcp_handlePacket()
{
	DHCP_Data d = (DHCP_Data)(ns.pbuf+DHCP_START);
	
	uint8_t *dhcp_op = ns.pbuf+DHCP_START+sizeof(DHCP_Data_t);
	
	//If the transaction ID isn't the last byte of our mac then the DHCP packet isn't for us
	//if(d->XID != ns.MAC[0])
	//	return;
	
	uint8_t i = 0, messageType = 0;
	
	//Get DHCP message type
	while(dhcp_op[i] != 0xFF)
	{
		if(dhcp_op[i] == DHCP_OP_MESSAGETYPE)
		{
			messageType = dhcp_op[i+2];
			break;
		}
		else
		{
			//Move to next op code
			i += dhcp_op[i+1] + 1;
		}			
	}
	
	//Handle the message depending on its type
	switch(messageType)
	{
		case DHCP_STATE_OFFER:
			//Save the IP the DHCP server offered
			for(uint8_t i = 0; i < 4; i++)
			{
				offeredIP[i] = d->yourIP[i];
			}
			
			dhcp_sendRequest();
		break;
		
		case DHCP_STATE_ACK:
			//Set out IP as the one ack'd by the DHCP server
			for(uint8_t i = 0; i < 4; i++)
			{
				ns.IP[i] = offeredIP[i];
			}
			//Set ipAcquired flag
			ns.ipAcquired = 1;
		break;
		
		default:
		
		break;
	}
}


/*! \fn void dhcp_buildPacket(uint8_t *pbuf)
 *  \brief Builds a DHCP packet WITHOUT any option codes
 *  \param pbuf  A pointer to the buffer to build the packet in. The pointer should point to the start of DHCP data.
 */
void dhcp_buildPacket(uint8_t *pbuf)
{
	DHCP_Data d = (DHCP_Data)(pbuf + DHCP_START);
	
	d->op = DHCP_REQUEST;
	d->htype = 1;//Ethernet
	d->hlen = 6;//Length of MAC Address
	d->hops = 0;//We should be talking directly to the server, no proxy needed.
	d->XID = ns.MAC[5];//Transaction ID is always the last number in the MAC address
	d->secs = 0;
	d->flags = 0;
	//Zero IP Addresses
	for(uint8_t i = 0; i < 4; i++)
	{
		d->clientIP[i] = 0x00;
		d->yourIP[i] = 0x00;
		d->serverIP[i] = 0x00;
		d->gatewayIP[i] = 0x00;		
	}
	//Send client MAC address
	for(uint8_t i = 0; i < 6; i++)
	{
		d->clientMAC[i] = ns.MAC[i];
	}
	//Zero hardware address padding
	for(uint8_t i = 6; i < 16; i++)
	{
		d->clientMAC[i] = 0x00;
	}
	//Zero server name
	for(uint8_t i = 0; i < 64; i++)
	{
		d->serverName[i] = 0x00;
	}
	//Zero boot file name
	for(uint8_t i = 0; i < 128; i++)
	{
		d->bootFilename[i] = 0x00;
	}
	//d->magicCookie = 0x63825363;
	d->magicCookie = 0x63538263;//The above number in little endian
	
	ns.plength += sizeof(DHCP_Data_t);
	
	//OP Codes follow...
}

//Documentation in header file
uint8_t dhcp_requestIP(uint16_t timeout_ms)
{
	//Get a timer
	int8_t timer = timer_get();
	timer_set(timer,timeout_ms);
	
	//Send dhcp discover packet
	dhcp_sendDiscover();

	do
	{
		//Wait for a packet to arrive
		//If no packets arrive in time, return 0
		while(enc28j60Read(EPKTCNT) == 0)
			if(timer_check(timer) == TIMER_EXPIRED)
			{
				//Release the timer
				timer_release(timer);
				return 0;
			}				
				
		//Receive packet
		ns.plength = enc28j60PacketReceive(ns.pbuf, ns.pbuf_size);
		
		//Check if packet is DHCP!
		if(ns.plength > 0)
			if(getPacketType() == DHCP)
				dhcp_handlePacket();
						
	}
	while(ns.ipAcquired == 0);	
	
	//Release the timer
	timer_release(timer);	
	
	//Return success		
	return ns.ipAcquired;
}