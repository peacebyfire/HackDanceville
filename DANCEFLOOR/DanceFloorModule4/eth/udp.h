/*
 * udp.h
 *
 * Created: 3/27/2013 2:43:19 PM
 *  Author: ben
 */ 


#ifndef UDP_H_
#define UDP_H_

#include <avr/io.h>

/** UDP Header Structure. */
typedef struct
{
	uint16_t senderPort; /** Sender Port */
	uint16_t targetPort; /** Target Port */
	uint16_t length;	 /** Length of entire UDP datagram in bytes, header+data */
	uint16_t checksum;	 /** Checksum */
	//Data...
} UDP_Data_t;

/** UDP Header Overlay. */
typedef UDP_Data_t *UDP_Data;

/*! \fn udp_makeHeader(uint8_t *pbuf, uint16_t senderPort, uint16_t targetPort, uint16_t length)
 *  \brief Creates a UDP header in pbuf
 *  \param pbuf  A pointer to where the header should start.
 */
void udp_makeHeader(uint8_t *pbuf, uint16_t senderPort, uint16_t targetPort);

void udp_setDataLength(uint8_t *pbuf, uint16_t dataLength);

void udp_computeChecksum(uint8_t *pbuf);

#endif /* UDP_H_ */