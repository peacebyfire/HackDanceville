/*
 * dfcontrol.h
 *
 * Created: 7/23/2012 11:57:55 AM
 *  Author: ben
 */ 


#ifndef DFCONTROL_H_
#define DFCONTROL_H_

#include <avr/io.h>

//extern uint8_t registered;

void init_dfControl();
//Check if this packet is a DanceFloor packet
//DF Packets begin with "DANCEFLOOR"
uint8_t isDFPacket(uint8_t *buf);
void handleDFPacket(uint8_t *buf);

//void sendTemp(uint8_t *buf);
//void sendAnnounce(uint8_t *buf);

void sendStatus(uint8_t *buf, uint8_t tileNum, uint16_t temperature);














#endif /* DFCONTROL_H_ */