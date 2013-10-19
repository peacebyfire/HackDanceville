/*
 * dhcp.h
 *
 * Created: 3/27/2013 3:44:54 PM
 *  Author: ben
 */ 


#ifndef DHCP_H_
#define DHCP_H_

#include <avr/io.h>

/**
 * DHCP Packet Structure
 */
typedef struct  __attribute__ ((__packed__))
{
	uint8_t  op;
	uint8_t  htype;
	uint8_t  hlen;
	uint8_t  hops;
	uint32_t XID;
	uint16_t secs;
	uint16_t flags;
	uint8_t  clientIP[4];
	uint8_t  yourIP[4];
	uint8_t  serverIP[4];
	uint8_t  gatewayIP[4];
	uint8_t  clientMAC[16];
	char     serverName[64];
	char     bootFilename[128];
	uint32_t magicCookie;
	
	//Option codes
	
} DHCP_Data_t;

/**
 * DHCP Packet Overlay
 */
typedef DHCP_Data_t *DHCP_Data;

#define DHCP_REQUEST	1
#define DHCP_REPLY		2

//Idle state used for this program only, NOT part of dhcp protocol
#define DHCP_IDLE	0

//DHCP Option Code Usage
// | DHCP_OP_CODE | Option Length | Option Data (of length Option Length) |
// | DHCP_OP_CODE | Option Length | Option Data (of length Option Length) | ...
// | DHCP_END     |

//DHCP Option Codes
#define DHCP_OP_MESSAGETYPE			53
	#define DHCP_STATE_DISCOVER	1
	#define DHCP_STATE_OFFER	2
	#define DHCP_STATE_REQUEST	3
	#define DHCP_STATE_DECLINE	4
	#define DHCP_STATE_ACK		5
	#define DHCP_STATE_NAK		6
	#define DHCP_STATE_RELEASE	7
	#define DHCP_STATE_INFORM	8
#define DHCP_OP_HOSTNAME			12
#define DHCP_OP_RENEWALTIMEVAL		58
#define DHCP_OP_REBINDINGTIMEVAL	59
#define DHCP_OP_IPLEASETIME			51
#define DHCP_OP_SERVERIP			54
#define DHCP_OP_CLIENTID			61
#define DHCP_OP_REQIPADDR			50
#define DHCP_OP_REQPARAMLIST		55
#define DHCP_OP_BCASTADDR			28

//End option code
#define DHCP_END				0xFF

#define DHCP_CLIENT_PORT	68
#define DHCP_SERVER_PORT	67

/*! \fn void dhcp_sendDiscover()
 *  \brief Makes a DHCP Discover packet to identify the DHCP server on the network
 */
void dhcp_sendDiscover();

/*! \fn void dhcp_sendRequest()
 *  \brief Makes a DHCP Request packet to request an IP address on the network.
 */
void dhcp_sendRequest();

/*! \fn void dhcp_handlePacket()
 *  \brief Handles a received DHCP packet.
 */
void dhcp_handlePacket();

/*! \fn void dhcp_requestIP(uint16_t timeout_ms)
 *  \brief Attempts to acquire an IP address using DHCP.
	DHCP Procedure:
	Send DHCP Discover
	Recv DHCP Offer
	Send DHCP Request
	Recv DHCP Ack
	If the time it takes to perform these four operations takes longer than timeout_ms the function will return zero.
	\param timeout_ms The maximum amount of time to wait after a dhcp request is sent.
	\return One if successful. Zero if unsuccessful.
 */
uint8_t dhcp_requestIP(uint16_t timeout_ms);





#endif /* DHCP_H_ */