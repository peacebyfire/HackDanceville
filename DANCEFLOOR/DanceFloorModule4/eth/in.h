/*
 * in.h
 *
 * Created: 3/28/2013 12:34:51 AM
 *  Author: ben
 */ 


#ifndef IN_H_
#define IN_H_

#include <avr/io.h>

uint32_t htonl(uint32_t hostlong);

uint16_t htons(uint16_t hostshort);

uint32_t ntohl(uint32_t netlong);

uint16_t ntohs(uint16_t netshort);


#endif /* IN_H_ */