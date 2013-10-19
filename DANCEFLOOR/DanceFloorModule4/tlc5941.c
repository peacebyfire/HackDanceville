/*
 * tlc5941.c
 *
 * Created: 2/15/2012 2:19:33 PM
 *  Author: ben
 */ 

#include "tlc5941.h"
#include "globals.h"
#include "delay.h"
#include <avr/eeprom.h>


void sendDotCorrect();

//Pin configuration
#define XERR_DDR	DDRD
#define XERR_PORT	PORTD
#define XERR_IN		PIND
#define XERR_PIN	0x04	//Bit mask for the port where the only high bit is the one corresponding to pin

#define SCLK_DDR	DDRD
#define SCLK_PORT	PORTD
#define SCLK_PIN	0x10

#define GSCLK_DDR	DDRD
#define GSCLK_PORT	PORTD
#define GSCLK_PIN	0x20

#define BLANK_DDR	DDRD
#define BLANK_PORT	PORTD
#define BLANK_PIN	0x80

#define GSDC_DDR	DDRB
#define GSDC_PORT	PORTB
#define GSDC_PIN	0x02

#define XLAT_DDR	DDRB
#define XLAT_PORT	PORTB
#define XLAT_PIN	0x01

#define MISO_DDR	DDRD
#define MISO_PORT	PORTD
#define MISO_PIN	0x01

#define MOSI_DDR	DDRD
#define MOSI_PORT	PORTD
#define MOSI_PIN	0x02

//SIN and SOUT pins are handled by configuring the serial interface

uint8_t frame[FRAME_SIZE];//Create full frame array
//uint8_t EEMEM dc_frame[DC_FRAME_SIZE];
uint8_t EEMEM dc_frame[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
							0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
						   };

uint8_t errorFlag = 0;
uint16_t thermalErrorFlags = 0;
uint8_t latchNextCycle = 0;
uint8_t ledOpenFlags[NUM_TLC5941*2];


void init_tlc5941()
{
	//Set pin DDRs
	//Outputs
	SCLK_DDR |= SCLK_PIN;
	GSCLK_DDR |= GSCLK_PIN;
	BLANK_DDR |= BLANK_PIN;
	GSDC_DDR |= GSDC_PIN;
	XLAT_DDR |= XLAT_PIN;
	MOSI_DDR |= MOSI_PIN;
	
	//Inputs
	MISO_DDR &= ~MISO_PIN;
	XERR_DDR &= ~XERR_PIN;
		
	//Enable XERR pull-up
	XERR_PORT |= XERR_PIN;
	
	
	//Enable USART SPI Mode (TLC5941's)
	//Enable master SPI mode, MSB first, sample on rising edge, setup on falling
	UCSR0C = 0b11000000;//Data output just before rising edge
	//Enable the TX and RX
	UCSR0B = 0b00011000;
	//UCSR0B = 0b00001000;//Just TX
	
	//Set clock to as fast as possible
	UBRR0H = 0;
	UBRR0L = 0;
	
	//zeroFrame();
	
	sendDotCorrect();
	
	init_GSCLK();
	GSCLK_start();
}


void init_GSCLK()
{
	TCCR0A = 0b00010010;//Toggle OC0B on compare match, CTC mode
	//OCRA is TOP
	TCCR0B = 0b00000000;//CTC mode (contd.), clock disconnected (initially)
	
	//Target: f_GSCLK = 2MHz
	
	//12.5 MHz / 2 / 3 => 2,083,333 Hz
	
	OCR0A = 3; //GSCLK is a little faster to keep in sync
	GSCLK_PORT &= ~GSCLK_PIN;//Make sure the pin state is low
	
	//Init Timer1
	//Enable timer1 to count GSCLK pulses (PD5)
	//Mimics 12-bit counter in TLC5941s
	//On TCNT == 0x0FFF do latch and reset routine
	//CTC mode, OCR1A is TOP,    WGM13-10 = 0b0100
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001111;//Use T1 GSCLK(PD5) pin as external clock, rising edge triggered
	TIMSK1 = 0b00000010;//Enable compare match interrupt (TCNT1 = OCR1A)
	OCR1A = 0x1000;//one more than the 12-bit max in TLC5941
	
}

void GSCLK_start()
{
	TCCR0B |= 0b00000001; //use CLK/1
}

void GSCLK_stop()
{
	TCCR0B &= 0b11111000;//disconnect CLK/1
	TCCR0B |= 0x40;
	GSCLK_PORT &= ~GSCLK_PIN;//Make sure the pin state is low
}

/* Data Frame Definition
 *	  12-bit   12-bit   12-bit       12-bit   12-bit   12-bit     288 bytes total
 *	|LED 0  B|LED 0  G|LED 0  R|...|LED 63 B|LED 63 G|LED 63 R|
 *   MSB..LSB
 *	 
 *	Channel 0 is at the beginning of the frame array
 */	
	

/*  setValue()
 *	Sets the value of the color of an led in the frame
 *
 *	The pattern repeats every two blocks of RGB data
 *	Frame
 *	|B|B|B|B|B|B|B|B|B|B|B|B|B|B|B|B|B|B|  bytes
 *	|B |G |R |B |G |R |B |G |R |B |G |R |  12-bit RGB values
 *	|    3   |   2    |    1   |    0   |  LED #
 *	| ONE REPETITION  |   ODD  |  EVEN  |
 *	
 *	First we determine if the LED # is even or odd by checking the least significant bit
 *	Then we use the color to determine the correct bit placement in the frame
 *	  The red and blue bit positions are similar
 *		
 *	The offset must be determined by whether the led # is even or odd and the color
 */
void setValue(uint8_t led, enum Color_t color, uint16_t value)
{
	led &= 0x3F;//Force LED <= 63
	
	uint16_t index;
			
	if(led & 0x01)//Odd
	{
		switch(color)
		{
			
			case RED:
				index = led*9/2;
				
				frame[index] = (frame[index] & 0xF0) | (((uint8_t)(value >> 8)) & 0x0F);
				index++;
				frame[index] = (uint8_t)(value);
							
			break;
				
			case GREEN:
				//index = FRAME_SIZE-7-(led >> 2)*9;//Calculate LSB index
				index = led*9/2+2;
				frame[index] = (uint8_t)(value >> 4);//MSB
				index++;
				frame[index] = (frame[index] & 0x0F) | (((uint8_t)(value << 4)) & 0xF0);//LSB

			break;
				
			case BLUE:
				//index = FRAME_SIZE-8-(led >> 2)*9;//Calculate LSB index
				index = led*9/2+3;
				frame[index] = (frame[index] & 0xF0) | (((uint8_t)(value >> 8)) & 0x0F);
				index++;
				frame[index] = (uint8_t)(value);
				
			break;
		}
	}			
	else//Even
	{
	
		switch(color)
		{
			case RED:
				index = led*9/2;
				frame[index] = (uint8_t)(value >> 4);//MSB
				index++;
				frame[index] = (frame[index] & 0x0F) | (((uint8_t)(value << 4)) & 0xF0);//LSB
			
			break;
				
			case GREEN:

				index = led*9/2+1;
				frame[index] = (frame[index] & 0xF0) | (((uint8_t)(value >> 8)) & 0x0F);
				index++;
				frame[index] = (uint8_t)(value);
								
			break;
				
			case BLUE:
				index = led*9/2+3;
				frame[index] = (uint8_t)(value >> 4);//MSB
				index++;
				frame[index] = (frame[index] & 0x0F) | (((uint8_t)(value << 4)) & 0xF0);//LSB
				
			break;
		}
	}
}
/*
uint8_t sendRecv(uint8_t tx)
{
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );
			
	//Send a byte
	UDR0 = tx;
			
	//Wait for data to be received
	while (!(UCSR0A & (1<<RXC0)));
			
	//Get received data
	return UDR0;
}*/

void send(uint8_t tx)
{
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );
			
	//Send a byte
	UDR0 = tx;
}

/*
 * sendFrame()
 *
 * This function clocks the full frame into the TLC5941 chain
 * 
 * Frame Information:
 * This is the LED numbering on the board, they are in one array of 12-bit words.
 * --------------------||--------------------
 * |  0 |  1 |  2 |  3 || 16 | 17 | 18 | 19 | 
 * |  4 |  5 |  6 |  7 || 20 | 21 | 22 | 23 |
 * |  8 |  9 | 10 | 11 || 24 | 25 | 26 | 27 |
 * | 12 | 13 | 14 | 15 || 28 | 29 | 30 | 31 |
 * ==========================================
 * | 48 | 49 | 50 | 51 || 32 | 33 | 34 | 35 |
 * | 52 | 53 | 54 | 55 || 36 | 37 | 38 | 39 |
 * | 56 | 57 | 58 | 59 || 40 | 41 | 42 | 43 |
 * | 60 | 61 | 62 | 63 || 44 | 45 | 46 | 47 |
 * --------------------||--------------------
 *
 * We translate the above numbering scheme to become
 * --------------------||--------------------
 * |  0 |  1 |  2 |  3 ||  4 |  5 |  6 |  7 |
 * |  8 |  9 | 10 | 11 || 12 | 13 | 14 | 15 |
 * | 16 | 17 | 18 | 19 || 20 | 21 | 22 | 23 |
 * | 24 | 25 | 26 | 27 || 28 | 29 | 30 | 31 |
 * ==========================================
 * | 32 | 33 | 34 | 35 || 36 | 37 | 38 | 39 |
 * | 40 | 41 | 42 | 43 || 44 | 45 | 46 | 47 |
 * | 48 | 49 | 50 | 51 || 52 | 53 | 54 | 55 |
 * | 56 | 57 | 58 | 59 || 60 | 61 | 62 | 63 |
 * --------------------||--------------------
 *
 * In this way, the control board is not overly complex
 * 
 */
//uint8_t tlc_num = NUM_TLC5941-1;
//uint8_t tlcByteCount = 0;
//uint8_t tlctemp;
uint8_t i,j;
uint8_t *fptr;
void sendFrame()
{

	//Make sure we are in grayscale mode
	GSDC_PORT &= ~GSDC_PIN;
	
	//If XERR_L is active set error flag
	/*
	if((XERR_IN & XERR_PIN) == 0)
	{
		errorFlag = 1;
	}		
	*/
	
	//Write first quadrant
	for(i = 0; i < 4; i++ )
	{
		fptr = frame + i*36;
		for(j = 0; j < 18; j++)
		{
			//sendRecv(frame[i*36+j]);
			send(*fptr);
			fptr++;
		}
	}
	
	//Write second quadrant
	for(i = 0; i < 4; i++ )
	{
		fptr = frame + i*36 + 18;
		for(j = 0; j < 18; j++)
		{
			//sendRecv(frame[18+i*36+j]);
			send(*fptr);
			fptr++;
		}
	}
	
	//Write third quadrant
	for(i = 0; i < 4; i++ )
	{
		fptr = frame + i*36 + 162;
		for(j = 0; j < 18; j++)
		{
			//sendRecv(frame[162+i*36+j]);
			send(*fptr);
			fptr++;
		}
	}
		
	for(i = 0; i < 4; i++ )
	{
		fptr = frame + i*36 + 144;
		for(j = 0; j < 18; j++)
		{
			//sendRecv(frame[144+i*36+j]);
			send(*fptr);
			fptr++;
		}
	}
	
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );
	

	
	
		/*
	for(uint16_t i = 0; i < FRAME_SIZE; i++)
	//for(uint16_t i = FRAME_SIZE-1; i >= 0; i--)
	{

		temp = sendRecv(frame[i]);
		
		//Handle received data
		if(tlcByteCount < 3)//Only deal with bytes 0,1,2
		{
			switch(tlcByteCount)
			{
				case 0://LOD 15:8, LED0 -> RGB,RGB ...
				ledOpenFlags[2*led_set] = temp;
				break;
				
				case 1://LOD 7:0
				ledOpenFlags[2*led_set+1] = temp;
				break;
				
				case 2://TEF
					if(temp & 0x80)//If TEF flag bit is set
					{
						thermalErrorFlags |= 1 << tlc_num;//Set corresponding TEF bit
					}
					else//If TEF bit is clear
					{
						thermalErrorFlags &= ~(1 << tlc_num);//Clear corresponding TEF bit
					}
				break;		
			}
		}
		
		if(++tlcByteCount >= 24)
		{
			tlcByteCount = 0;
			tlc_num--;
			led_set++;
		}
	}
	*/
}

/*
 * zeroFrame()
 *
 * This function sets the frame to all zeros
 *
 */
void zeroFrame()
{
	uint16_t i;	
	for(i = 0; i < FRAME_SIZE; i++)
	{
		frame[i] = 0x00;
	}
}

void sendDotCorrect()
{
	GSCLK_stop();

	delay_ms(1);

	//Make sure we are in Dot Correct mode
	GSDC_PORT |= GSDC_PIN;
	
	//delay_us(100);
	
	/*
	for(uint8_t i = 0; i < DC_FRAME_SIZE; i++)
	{
		// Wait for empty transmit buffer
		while ( !( UCSR0A & (1<<UDRE0)) );
	
		//Get byte from EEPROM and send
		UDR0 = eeprom_read_byte(dc_frame+i);
	}	
	
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	*/
	/*
		for(i = 0; i < 4; i++ )
	{
		fptr = frame + i*36;
		for(j = 0; j < 18; j++)
		{
			//sendRecv(frame[i*36+j]);
			send(*fptr);
			fptr++;
		}
	}
	*/
	
	delay_ms(1);
	
	uint8_t *dc_ptr = dc_frame;
		
	//Write first quadrant
	for(i = 0; i < 4; i++ )//For each row in the quadrant
	{
		dc_ptr = dc_frame + i*18;
		for(j = 0; j < 9; j++)
		{
			//sendRecv(frame[i*36+j]);
			send(eeprom_read_byte(dc_ptr));
			dc_ptr++;
		}
	}
	
	//Write second quadrant
	for(i = 0; i < 4; i++ )
	{
		dc_ptr = dc_frame + i*18 + 9;
		for(j = 0; j < 9; j++)
		{
			//sendRecv(frame[18+i*36+j]);
			send(eeprom_read_byte(dc_ptr));
			dc_ptr++;
		}
	}
	
	//Write third quadrant
	for(i = 0; i < 4; i++ )
	{
		dc_ptr = dc_frame + i*18 + 81;
		for(j = 0; j < 9; j++)
		{
			//sendRecv(frame[162+i*36+j]);
			send(eeprom_read_byte(dc_ptr));
			dc_ptr++;
		}
	}
		
	for(i = 0; i < 4; i++ )
	{
		dc_ptr = dc_frame + i*18 + 72;
		for(j = 0; j < 9; j++)
		{
			//sendRecv(frame[144+i*36+j]);
			send(eeprom_read_byte(dc_ptr));
			dc_ptr++;
		}
	}
	
	
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	
	delay_ms(1);
	
	
	
	
	//Set XLAT High (pulse duration >= 20 ns, 10 MHz => 100 ns per clock cycle)
	XLAT_PORT |= XLAT_PIN;
		
	//delay_us(100);
	delay_ms(1);
		
	//Set XLAT Low
	XLAT_PORT &= ~XLAT_PIN;
	
	//delay_us(100);
	delay_ms(1);
	
	//Switch back to grayscale mode
	GSDC_PORT &= ~(GSDC_PIN);
	
	GSCLK_start();

}


/*
 * setDotCorrect()
 *
 * This function sets the dot correct values in EEPROM and sends them to the TLCs
 * The buf points to DC_FRAME_SIZE bytes to use for dot correct values
 *
 * DC data is 8-bit coming in. This function truncates the most significant 2 bits and
 * packs the remaining 6-bit values together in the array in EEPROM at dc_frame.
 *
 * The pattern repeats every 3 bytes
 * |B  |B  |B  |	Bytes
 * |D |D |D |D |	6-bit DC Values
 *
 *
 */
void setDotCorrect(uint8_t *buf)
{
	//Each block of 3 bytes holds 4 6-bit DC values
	uint8_t temp[3];
	
	for(uint8_t ledChannel = 0; ledChannel < NUM_TLC5941*16; ledChannel+=4)
	{
		temp[0] = buf[ledChannel] << 2;

		temp[0] |= (buf[ledChannel+1] & 0x3F) >> 4;
		temp[1] = buf[ledChannel+1] << 4;

		temp[1] |= (buf[ledChannel+2] & 0x3F) >> 2;
		temp[2] = buf[ledChannel+2] << 6;

		temp[2] |= buf[ledChannel+3] & 0x3F;

		eeprom_write_block(temp, dc_frame+((ledChannel/4)*3), 3);
	}
	
	sendDotCorrect();
}

/*
 * getDotCorrect()
 *
 * This function gets DC_FRAME_SIZE  bytes of dot correct values from EEPROM and puts them in buf
 *	NEEDS TO CONVERT 6-BIT TO 8 BIT BYTES
 */

void getDotCorrect(uint8_t *buf)
{
	//Each block of 3 bytes holds 4 6-bit DC values
	uint8_t temp[3];
	
	//Read DC values from EEPROM
	for(uint8_t ledChannel = 0; ledChannel < NUM_TLC5941*16; ledChannel+=4)
	{
		//Read the block containing the next 4 ledChannel DC values
		eeprom_read_block(temp, dc_frame+ledChannel/4, 3);
		
		buf[ledChannel]   = temp[0] >> 2;
		buf[ledChannel+1] = ((temp[0] & 0x03) << 4) | (temp[1] >> 4);
		buf[ledChannel+2] = ((temp[1] & 0x0F) << 2) | (temp[2] >> 6);
		buf[ledChannel+3] = temp[2] & 0x3F;
	}		
}

//DotCorrect data is 16 6-bit words per TLC5941
//0 = low brightness
//63 = full brightness


/* 
 *	This function resets the GS timer after a GS cycle and sends the latch signal if new data has been clocked in
 *	The signal to latch is determined by the update flag. A latch will occur if update == 1.
 *
 *  ISR for Timer1 compare match (TLC5941 timer MAX reached)
 */
ISR(TIMER1_COMPA_vect)
{
	//Stop GSCLK
	GSCLK_stop();
	
	//Set BLANK High, clears TLC's GS counter
	BLANK_PORT |= BLANK_PIN;
	
	//clear TCNT1, the timer that keeps track of the TLC's GS counter
	TCNT1 = 0x0000;
	
	//Update if needed
	if(latchNextCycle)
	{
		//Frame should be previously written, latch data
		//Set XLAT High (pulse duration >= 20 ns, 10 MHz => 100 ns per clock cycle)
		XLAT_PORT |= XLAT_PIN;

		//Set XLAT Low
		XLAT_PORT &= ~XLAT_PIN;
		
		//Clear update flag
		latchNextCycle = 0;
	}
		
	//Set BLANK Low
	BLANK_PORT &= ~BLANK_PIN;
	
	//Start GSCLK
	GSCLK_start();
}

