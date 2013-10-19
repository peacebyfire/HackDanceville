/*
 * temp.h
 *
 * Created: 7/24/2012 12:45:55 PM
 *  Author: ben
 */ 


#ifndef TEMP_H_
#define TEMP_H_

#include <avr/io.h>


void init_temp();

//Returns the temperature sensor reading in deg C/512
uint16_t getTemp();

uint16_t getTempRaw();


#endif /* TEMP_H_ */