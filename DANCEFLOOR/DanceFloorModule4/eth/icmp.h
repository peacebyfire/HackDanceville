/*
 * icmp.h
 *
 * Created: 3/27/2013 4:15:32 PM
 *  Author: ben
 */ 


#ifndef ICMP_H_
#define ICMP_H_

#include <avr/io.h>

typedef struct  
{
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	uint16_t identifier;
	uint16_t sequenceNumber;
	
}ICMP_Data_t;

typedef ICMP_Data_t *ICMP_Data;

#define PING_REQUEST	8
#define PING_REPLY		0

void icmp_sendPingReply();

void icmp_handlePacket();

void icmp_computeChecksum();


#endif /* ICMP_H_ */