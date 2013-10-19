/*
 * packetType.h
 *
 * Created: 3/28/2013 8:00:43 PM
 *  Author: ben
 */ 


#ifndef PACKETTYPE_H_
#define PACKETTYPE_H_

typedef enum {ARP, DHCP, ICMP, DF, Unknown} PacketType;


/*! \fn PacketType getPacketType()
 *  \brief Returns the type of packet currently in the buffer.
 */
PacketType getPacketType();


#endif /* PACKETTYPE_H_ */