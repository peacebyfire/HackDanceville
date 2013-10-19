/*
 * tlc5941.h
 *
 * Created: 2/15/2012 2:19:41 PM
 *  Author: ben
 */ 


#ifndef TLC5941_H_
#define TLC5941_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "globals.h"

//This flag is set if an error occurs
extern uint8_t errorFlag;
//Set this flag to latch data at the end of the current GS cycle
extern uint8_t latchNextCycle;

#define NUM_TLC5941		12             //12 for a full block
#define FRAME_SIZE		24*NUM_TLC5941 //288 bytes for full tile
#define DC_FRAME_SIZE	12*NUM_TLC5941 //144 bytes for full tile


//Grayscale frame
extern uint8_t frame[FRAME_SIZE];//Create full frame array

//Dot-Correct frame in EEPROM
extern uint8_t EEMEM dc_frame[DC_FRAME_SIZE];

//Thermal Error Flags 15:0 bits (one bit per TLC5941)
extern uint16_t thermalErrorFlags;

//LED Open Detection (LOD15:LOD0) per TLC
//LED0 -> LED63 RGB,RGB... bits
extern uint8_t ledOpenFlags[NUM_TLC5941*2];


void init_tlc5941();

//-----GSCLK Related-----
//****INLINE THESE!
void init_GSCLK();

void GSCLK_start();
void GSCLK_stop();
//-----------------------

enum Color_t {RED = 0, GREEN = 1, BLUE = 2};

void setValue(uint8_t led, enum Color_t color, uint16_t value);

void zeroFrame();

void sendFrame();

void setDotCorrect(uint8_t *buf);



//void getDotCorrect(uint8_t *buf);

#ifdef DEBUG_TLC
void printFrame();
#endif




#endif /* FRAME_H_ */