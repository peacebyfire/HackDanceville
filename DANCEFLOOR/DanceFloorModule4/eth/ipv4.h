/*
 * ipv4.h
 *
 * Created: 3/27/2013 3:09:06 PM
 *  Author: ben
 */ 


#ifndef IPV4_H_
#define IPV4_H_

#include <avr/io.h>

/** IPv4 Header Structure. */
typedef struct __attribute__ ((__packed__))
{
	uint8_t IPver4_Length4;			/** IP Version and Header Length (# of 32-bit words in header)*/
	uint8_t  typeOfService;			/** Differentiated Services Field */
	uint16_t totalLength;			/** Total length (in bytes, including header) */
	uint16_t identification;		/** Identification field. Primarily used for uniquely identifying fragments of an original IP datagram */
	uint16_t flags3_offset13;		/** This flag is always zero | Don't fragment flag | More fragments flag (used when a packet is fragmented) | Offset of a particular fragment relative to the beginning of the original unfragmented IP datagram (8-byte blocks) */
	uint8_t  ttl;					/** Time-to-Live */
	uint8_t  protocol;				/** Protocol */
	uint16_t checksum;				/** Checksum */
	uint8_t  senderIP[4];			/** IP Address of sender */
	uint8_t  targetIP[4];			/** IP Address of target */
} IPv4_Data_t;

/** IPv4 Header Overlay. */
typedef IPv4_Data_t *IPv4_Data;

#define IPVER_HDRLEN	0x45

/** IP Protocols */
typedef enum { IP_ICMP = 0x01, /*!< The packet is an ICMP packet */
			   IP_UDP  = 0x11  /*!< The packet is a UDP packet */
			 }IP_PROTOCOL;
			 
extern uint8_t zeroIP[4];
extern uint8_t bcastIP[4];

/*! \fn void ip_makeHeader(uint8_t *pbuf, IP_PROTOCOL protocol, uint8_t *senderIP, uint8_t *targetIP)
 *  \brief Makes an IP header with the given information.
 *	This function fills in the basic IP header information.
 *  It sets totalLength and checksum to zero. These must be updated using ip_setDataLength() and ip_computeChecksum()
 *
 *  \param pbuf  A pointer to the buffer which will contain the header.
 *  \param protocol The protocol to use.
 *  \param senderIP The IP of the sender.
 *  \param targetIP The target IP address.
 */
void ip_makeHeader(uint8_t *pbuf, IP_PROTOCOL protocol, uint8_t *senderIP, uint8_t *targetIP);

/*! \fn void ip_setDataLength(uint8_t *pbuf, uint16_t dataSize)
 *  \brief Sets the data size for an IP packet.
 *  \param pbuf  A pointer to the buffer which will contain the header.
 *  \param dataSize The size of the data within the IP packet. This doesn't include the header.
 */
void ip_setDataLength(uint8_t *pbuf, uint16_t dataLength);

/*! \fn void ip_computeChecksum(uint8_t *pbuf, uint16_t dataLength)
 *  \brief Computes the checksum for a complete IP header.
 *  \param pbuf  A pointer to the buffer which will contain the header.
 */
void ip_computeChecksum(uint8_t *pbuf);

#endif /* IPV4_H_ */