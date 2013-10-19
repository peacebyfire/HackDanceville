/*
 * eth.h
 *
 * Created: 3/27/2013 3:06:20 PM
 *  Author: ben
 */ 


#ifndef ETH_H_
#define ETH_H_

/** Ethernet Header Structure */
typedef struct  __attribute__ ((__packed__))
{
	uint8_t  destMAC[6];			/** Target MAC Address */
	uint8_t  srcMAC[6];				/** Sender MAC Address */
	uint16_t ptype;					/** Packet Type */
}ETH_Data_t;

/** Ethernet Header Overlay */
typedef ETH_Data_t *ETH_Data;

/** Ethernet packet types */
typedef enum  { ETH_ARP = 0x0806,	/*!< The packet is an ARP packet */
				ETH_IP  = 0x0800	/*!< The packet is an IP packet  */		
			  }ETH_PTYPE;
			  
extern uint8_t bcastMAC[6];
					   

/*! \fn eth_getPacketType(uint8_t *packet)
 *  \brief Returns the type of packet specified in the ethernet frame.
 *
 *  \param pbuf A pointer to the buffer containing the ethernet packet data.
 *
 *  \return The type of packet
 */
ETH_PTYPE eth_getPacketType(uint8_t *pbuf);

/*! \fn eth_makeHeader(uint8_t *srcMAC, uint8_t *destMAC, ETH_PTYPE ptype)
 *  \brief Makes an ethernet header with the given information.
 *
 *  \param pbuf  A pointer to the buffer which will contain the header.
 *  \param srcMAC  The source MAC address.
 *  \param destMAC The destination MAC address.
 *  \param ptype   The desired type of packet.
 */
void eth_makeHeader(uint8_t *pbuf, uint8_t *srcMAC, uint8_t *destMAC, ETH_PTYPE ptype);






#endif /* ETH_H_ */