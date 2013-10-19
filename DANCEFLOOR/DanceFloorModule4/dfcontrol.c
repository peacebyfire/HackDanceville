/*
 * dfcontrol.c
 *
 * Created: 7/23/2012 11:57:43 AM
 *  Author: ben
 */ 


#include <avr/io.h>
#include <avr/eeprom.h>

#include "dfcontrol.h"
#include "eth/net.h"
#include "tlc5941.h"
#include "globals.h"
#include "temp.h"




//----------DF Packet Types----------
#define DF_PING				0x00
#define DF_PONG				0x01

#define DF_ANNOUNCE			0xA0
#define DF_ANNOUNCEREQ		0xAA
#define DF_ANNOUNCEACK		0xA1

#define DF_SETMAC			0x10
#define DF_SETIP			0x11
#define DF_SETNUM			0x12
#define DF_RESTART			0xFF

#define DF_GETSTATUS		0x15
#define DF_STATUS			0x16

#define DF_SETFRAME			0x20
#define DF_DISPFRAME		0x21

#define DF_SETDOTCORRECT	0x30

#define DF_GETTEMP			0x40
#define DF_TEMP				0x41
//-----------------------------------

#define DF_OP		10	//Location of OP in DF packet
#define DF_DATA		11	//Location of data in DF packet


//uint8_t registered = 0;//Registered if host has acknowledged

//uint8_t* broadcastip = {192,168,1,255};
/*
uint8_t *DF_text = "DANCEFLOOR";
uint8_t temp[150];
*/
void init_dfControl()
{
	/*
	//registered = 0;
	
	for(uint8_t i = 0; i < DF_OP; i++)
	{
		temp[i] = DF_text[i];
	}
	*/
}

/*
//Internal handler functions
void sendPingResponse(uint8_t *buf)
{
	temp[DF_OP] = DF_PONG;
	//make_udp_reply_from_request(buf, temp, 11, DF_PORT);
}

*/
/*
void sendStatus(uint8_t *buf, uint8_t tileNum, uint16_t temperature)
{

	//|"DANCEFLOOR"|DF_STATUS|TileNum|Temp_C/10_H|Temp_C/10_L|TEF_H|TEF_L|LOD_H0|LOD_L0|LOD_H1|LOD_L1|...|LOD_H11|LOD_L11|
	temp[DF_OP] = DF_STATUS;
	
	//Fill in TileNum
	temp[DF_DATA] = tileNum;
	
	//Fill in temp
	temp[DF_DATA+1] = temperature / 256;
	temp[DF_DATA+2] = temperature; 
	
	//Fill TEF Data
	temp[DF_DATA+3] = (uint8_t)(thermalErrorFlags >> 8);
	temp[DF_DATA+4] = (uint8_t)(thermalErrorFlags);
	
	//Fill LOD Data
	for(uint8_t i = 0; i < NUM_TLC5941*2; i++)
	{
		temp[DF_DATA+5+i] = ledOpenFlags[i];
	}
	
	//make_udp_reply_from_request(buf, temp, 16+NUM_TLC5941*2, DF_PORT);//11 w/o data
}*/

/*
void handleSetMAC(uint8_t *buf)
{
	//Set in EEPROM, restart
	uint8_t *mac = buf+UDP_DATA_P+DF_DATA;
	eeprom_write_block(mac,EEmac,8);
	
	//Restart (Loop until watchdog timer expires)
	while(1);
}
void handleSetIP(uint8_t *buf)
{
	//Set in EEPROM, restart
	uint8_t *ip = buf+UDP_DATA_P+DF_DATA;
	eeprom_write_block(ip,EEip,4);
	
	//Restart (Loop until watchdog timer expires)
	while(1);	
}
void handleSetNum(uint8_t *buf)
{
	//Set in EEPROM
	uint8_t num = buf[UDP_DATA_P+DF_DATA];
	eeprom_write_byte(&EEtileNum,num);
}
void handleRestart(uint8_t *buf)
{
	//Restart (Loop until watchdog timer expires)
	while(1);
}
*/
/*
void handleSetFrame(uint8_t *buf)
{
	uint8_t *ledVal = buf+UDP_DATA_P+DF_DATA;
	uint16_t ledOffset;
	uint16_t temp;
	uint8_t *temp_arr = &temp;

	for(uint8_t led = 0; led < 64; led++)
	{
		ledOffset = led * 6;
		//Convert big endian values to little endian
		temp_arr[0] = ledVal[ledOffset+1];
		temp_arr[1] = ledVal[ledOffset];
		setValue(led,RED,   temp);
				
		temp_arr[0] = ledVal[ledOffset+3];
		temp_arr[1] = ledVal[ledOffset+2];
		setValue(led,GREEN, temp);
				
		temp_arr[0] = ledVal[ledOffset+5];
		temp_arr[1] = ledVal[ledOffset+4];
		setValue(led,BLUE,  temp);
	}
	
	//Uses same port (prints in function instead of writing to port)
	sendFrame();
}
*/
void handleDisplayFrame(uint8_t *buf)
{
	latchNextCycle = 1;
}
/*
void handleSetDotCorrect(uint8_t *buf)
{
	setDotCorrect(buf+UDP_DATA_P+DF_DATA);
}
*/
/*
//Checks if packet begins with "DANCEFLOOR"
uint8_t isDFPacket(uint8_t *buf)
{
	for(uint8_t i = 0; i < 10; i++)
	{
		if(buf[UDP_DATA_P+i] != DF_text[i])
		{
			return 0;//False
		}
	}
	return 1;//True
}
*/
/*
void handleDFPacket(uint8_t *buf)
{
	#ifdef DEBUG_DFCONTROL
		printString("Handling DF Packet\n");
	#endif
	//Isolate df packet portion of full udp packet
	//uint8_t *df_packet = buf+UDP_DATA_P;
	
	switch(buf[DF_OP+UDP_DATA_P])
	{
		case DF_PING:
			sendPingResponse(buf);
			break;
		case DF_SETMAC:
			handleSetMAC(buf);
			break;
			
		case DF_SETIP:
			handleSetIP(buf);
			break;
		
		case DF_SETNUM:
			handleSetNum(buf);
			break;
			
		case DF_RESTART:
			handleRestart(buf);
			break;
		
		case DF_GETSTATUS:
			sendStatus(buf);
			break;
		
		case DF_SETFRAME:
			handleSetFrame(buf);
			break;
		
		case DF_DISPFRAME:
			handleDisplayFrame(buf);
			break;
			
		case DF_SETDOTCORRECT:
			handleSetDotCorrect(buf);
			break;
		
		case DF_GETTEMP:
			sendTemp(buf);
			break;	
		
	}
}
*/
/*
void sendAnnounce(uint8_t *buf)
{
	#ifdef DEBUG_DFCONTROL
		printString("Sending announce\n");
	#endif
	
	temp[DF_OP] = DF_ANNOUNCE;
	temp[DF_DATA] = tileNum;
	//buf[UDP]
	//make_udp_reply_from_request(buf, temp, 12, DF_PORT);
	
	send_udp_bcast(buf, temp, 12, DF_PORT, broadcastip, DF_PORT+1);
}
*/

/*
void sendTemp(uint8_t *buf)
{
	#ifdef DEBUG_DFCONTROL
		printString("Sending temperature\n");
	#endif
	temp[DF_OP] = DF_TEMP;
	//uint16_t T = getTempRaw();
	//temp[DF_DATA] = (uint8_t)(T >> 8);
	//temp[DF_DATA+1] = (uint8_t)(T);
	temp[DF_DATA] = getTemp();
	make_udp_reply_from_request(buf, temp, 12, DF_PORT);
}
*/
/*
void handleAnnounceREQ(uint8_t *buf)
{
	temp[DF_OP] = DF_ANNOUNCEREQ;
	temp[DF_DATA] = tileNum;

	make_udp_reply_from_request(buf, temp, 13, DF_PORT);
	
	//registered = 1;		
}
*/
/*
void handleAnnounceACK(uint8_t *buf)
{
	
	temp[DF_OP] = DF_ANNOUNCEACK;
	temp[DF_DATA] = tileNum;

	make_udp_reply_from_request(buf, temp, 13, DF_PORT);
	
	
	registered = 1;	
}
*/