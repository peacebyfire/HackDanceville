/*
 * df.c
 *
 * Created: 3/30/2013 3:59:01 PM
 *  Author: ben
 */ 

#include "df.h"
#include "eth/net.h"
#include "eth/eth.h"
#include "eth/arp.h"
#include "eth/ipv4.h"
#include "eth/udp.h"
#include "eth/in.h"
#include "eth/enc28j60.h"
#include "tlc5941.h"

#define DF_START		sizeof(ETH_Data_t)+sizeof(IPv4_Data_t)+sizeof(UDP_Data_t)
//#define DF_DATA_START	sizeof(ETH_Data_t)+sizeof(IPv4_Data_t)+sizeof(UDP_Data_t)+sizeof(DF_Header_t)

char DF_Text[] = "DANCEFLOOR";

/*
DF_Header dh;
DF_Frame df;
DF_DotCorrect ddc;
*/

uint16_t df_temp;
uint8_t *dfd;
uint8_t df_handlePacket()
{
	//dh = (DF_Header)(ns.pbuf + DF_START);
	dfd = (ns.pbuf + DF_START);
	
	//Make sure this is a DANCEFLOOR packet
	for(uint8_t i = 0; i < 10; i++)
		if(*(dfd++) != DF_Text[i])
			return;
			
		
	switch(*dfd)
	{
		case DF_Stat:
			//TODO
		break;
		
		case DF_SetFrame:
			//df = (DF_Frame)(ns.pbuf + DF_DATA_START);
			dfd = (ns.pbuf + DF_START + DF_DATA_P);
			for(uint8_t led = 0; led < 64; led++)
			{
				df_temp = *dfd;
				df_temp = df_temp << 4;
				dfd++;
				setValue(led, RED,   df_temp);
				df_temp = *dfd;
				df_temp = df_temp << 4;
				dfd++;
				setValue(led, GREEN, df_temp);
				df_temp = *dfd;
				df_temp = df_temp << 4;
				dfd++;
				setValue(led, BLUE,  df_temp);
			}				
			sendFrame();
		break;
		
		case DF_DispFrame:
			latchNextCycle = 1;
		break;
		
		case DF_SetDotCorrect:
			setDotCorrect(ns.pbuf + DF_START + DF_DATA_P);
		break;
		
		case DF_GetDotCorrect:
			//TODO
		break;
		
		
		case DF_Announce:
			return 0;//The packet wasn't for us
			
		default:
			return 0;
	}
	
	return 1;//The packet is for us
}

void df_announce()
{
	//Build ethernet header
	eth_makeHeader(ns.pbuf, ns.MAC, bcastMAC, ETH_IP);
	//Build IP header
	ip_makeHeader(ns.pbuf, IP_UDP, ns.IP, bcastIP);
	//Build UDP header
	udp_makeHeader(ns.pbuf, DF_PORT, DF_PORT);

		
	//Fill in op-codes
	uint8_t *data = ns.pbuf + DF_START;
	for(uint8_t i = 0; i < 10; i++)
		data[i] = DF_Text[i];
		
	data[10] = DF_Announce;
		
	data[11] = tileNum;
	
	for(uint8_t i = 0; i < 4; i++)
		data[i+12] = ns.IP[i];
			
	ns.plength += 16;
		
	udp_setDataLength(ns.pbuf, 15);
	udp_computeChecksum(ns.pbuf);
		
	ip_setDataLength(ns.pbuf, sizeof(UDP_Data_t)+15);
	ip_computeChecksum(ns.pbuf);
		
	enc28j60PacketSend(ns.pbuf, ns.plength);
}