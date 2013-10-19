/*
 * in.c
 *
 * Created: 3/28/2013 12:34:57 AM
 *  Author: ben
 */ 

//Host is little endian, network is big endian

#include "in.h"
#include <avr/io.h>

uint32_t htonl(uint32_t hostlong)
{
	return (hostlong << 24) | ((hostlong << 8) & 0x00FF0000) | ((hostlong >> 8) & 0x0000FF00) | (hostlong >> 24);
}

uint16_t htons(uint16_t hostshort)
{
	return (hostshort << 8) | (hostshort >> 8);
}

uint32_t ntohl(uint32_t netlong)
{
	return (netlong << 24) | ((netlong << 8) & 0x00FF0000) | ((netlong >> 8) & 0x0000FF00) | (netlong >> 24);
}

uint16_t ntohs(uint16_t netshort)
{
	return (netshort << 8) | (netshort >> 8);
}
