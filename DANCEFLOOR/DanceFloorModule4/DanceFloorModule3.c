/*
 * DanceFloorModule.c
 *
 * Created: 7/20/2012 12:10:45 PM
 *  Author: ben
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <stdlib.h>

#include "globals.h"
#include "delay.h"
#include "systimer.h"
#include "eth/net.h"

#include "temp.h"
//#include "dfcontrol.h"
#include "df.h"
#include "idlePattern.h"
#include "tlc5941.h"

#include "eth/enc28j60.h"//For reading packets
#include "eth/packetType.h"
#include "eth/dhcp.h"

#include "eth/ipv4.h"
#include "eth/udp.h"
#include "eth/eth.h"
#include "eth/arp.h"

/*
typedef struct 
{
	ETH_Data_t eth;
	IPv4_Data_t ip;
	UDP_Data_t udp;
	DHCP_Data_t dhcp;
} PacketOverlay_t;

volatile PacketOverlay_t *p;
*/
	
//***************************** Timer System **********************************
#define DHCP_TIMEOUT			2000
#define ANNOUNCE_INTERVAL		1000 //Milliseconds
#define IDLE_TIMEOUT			2000 //Milliseconds
#define IDLE_FRAMEDELAY			50  //Milliseconds

	

//*****************************************************************************

uint8_t EEMEM EEtileNum = DF_TILENUM;
uint8_t tileNum = DF_TILENUM;

//Ethernet Packet Buffer
#define ETH_BUF_SIZE 500
static uint8_t pbuf[ETH_BUF_SIZE];


void checkTemp();
//void handlePackets();
//uint8_t Connect();//Returns 0 if not connected, 1 if connected
void init_WDT();

uint8_t connected;

//Used to track delays between DF packets leading up to standalone mode
//uint8_t DFPacketReceived = 0;

uint16_t temp;
PacketType type;
int8_t announceTimer, idleTimer, idleFrameTimer;
int main(void)
{
	//p = (PacketOverlay_t *)pbuf;
	connected = 0;
	
	//Initialize tileNum from EEPROM
	tileNum = eeprom_read_byte(&EEtileNum);
	
	init_net(pbuf, ETH_BUF_SIZE);
	init_enc28j60(ns.MAC);
	//init_dfControl();
	init_tlc5941();
	init_timer();
	init_temp();
	sei();//Enable global interrupts
	wdt_reset();
	//init_WDT();
	
	//Configure timers
	announceTimer = timer_get();
	timer_set(announceTimer, ANNOUNCE_INTERVAL);
	idleTimer = timer_get();
	timer_set(idleTimer, IDLE_TIMEOUT);
	idleFrameTimer = timer_get();
	
	//*******************************MAIN LOOP BEGIN*********************************
		
	while(1)
	{
		//sei();//Just to make sure
		wdt_reset();
		
		if(enc28j60linkup())
		{
			//While we haven't obtained an IP, attempt to obtain one via dhcp
			if(ns.ipAcquired == 0)
			{
				dhcp_requestIP(DHCP_TIMEOUT);
			}
			else//If we have a network address
			{

				//If a packet has been received and is in the ENC28J60's buffer
				if(enc28j60Read(EPKTCNT) > 0)
				{
					//Receive the packet
					ns.plength = enc28j60PacketReceive(ns.pbuf, ns.pbuf_size);

					if(ns.plength == 0)
						continue;
					
					//Get the packet type
					type = getPacketType();
					
					switch(type)
					{
						case ARP:
							arp_handlePacket();
						break;
						
						case DF:
							//Returns 1 if the packet was for us
							if(df_handlePacket())
							{
								//Reset announce timer
								timer_set(announceTimer, ANNOUNCE_INTERVAL);
								//Reset idle timer
								timer_set(idleTimer, IDLE_TIMEOUT);
							}							
						break;
						
						case DHCP:
						case ICMP:
						case Unknown:
						default:
						
						break;
					}		
								
				}
				else
				{
					//Check announce timer
					if(timer_check(announceTimer) == TIMER_EXPIRED)
					{
						//Announce our presence on the network
						df_announce();
						//Reset announce timer
						timer_set(announceTimer, ANNOUNCE_INTERVAL);	
					}
					
					if(timer_check(idleTimer) == TIMER_EXPIRED)
					{
						if(timer_check(idleFrameTimer) == TIMER_EXPIRED)
						{
							idlePattern1_ComputeFrame();
							sendFrame();
							latchNextCycle= 1;
							timer_set(idleFrameTimer, IDLE_FRAMEDELAY);
						}
					}
					
					
				}
				
				
				
			}
		}
		else//If the link is detected down
		{
			//If the link goes down, invalidate our IP
			ns.ipAcquired = 0;
			
			//If link is down, the only mode is idle mode
			if(timer_check(idleFrameTimer) == TIMER_EXPIRED)
			{
				idlePattern1_ComputeFrame();
				sendFrame();
				latchNextCycle= 1;
				timer_set(idleFrameTimer, IDLE_FRAMEDELAY);
			}
			
		}
	}
	
	

}


//initialize watchdog timer
void init_WDT(void)
{
	//disable interrupts
	cli();
	//reset watchdog
	wdt_reset();
	
	//Clear WDRF in MCUSR
	MCUSR &= ~(1<<WDRF);
	//Enable prescaler changes
	WDTCSR |= (1<<WDCE)|(1<<WDE);
	//set up WDT reset on timeout with 4s prescaler
	WDTCSR = (1<<WDE)|(1<<WDP3);
	//Enable global interrupts
	sei();
}
