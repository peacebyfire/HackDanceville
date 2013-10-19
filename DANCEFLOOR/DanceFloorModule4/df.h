/*
 * df.h
 *
 * Created: 3/30/2013 3:59:07 PM
 *  Author: ben
 */ 


#ifndef DF_H_
#define DF_H_

#include <avr/io.h>

typedef enum {DF_Stat = 0x00, DF_SetFrame = 0x01, DF_DispFrame = 0x02, DF_SetDotCorrect = 0x11, DF_GetDotCorrect  = 0x12, DF_Announce = 0xFF} DF_OP;


/*
typedef struct  
{
	char DF[10];	
	uint8_t op;		
} DF_Header_t;
*/
#define DF_OP_P		10
#define DF_DATA_P	11

/** DanceFloor Packet Header Overlay. */
//typedef DF_Header_t *DF_Header;

/*

typedef struct
{
	uint8_t data[192]; 
} DF_Frame_t;

typedef DF_Frame_t *DF_Frame;
*/

//Returns 1 if the packet was for us, 0 if it was not
uint8_t df_handlePacket();

void df_announce();



#endif /* DF_H_ */