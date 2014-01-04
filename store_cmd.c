/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <twi.h>
#include <gpio.h>
#include <stdio.h>
#define nanocam_addr 0x06 //Nanocam address

int main (void)
{
	board_init();
	
	//Pin configurations
	
	//Make them Peripheral lines
	AVR32_GPIO.port[0].gper &= ~((1<<29) | (1<<30));			//Clears 29th & 30th bits (makes them peripheral pins)
	AVR32_GPIO.port[0].pmr0 &= ~((1<<29) | (1<<30));			//Sets them as I2C pins
	AVR32_GPIO.port[0].pmr1 &= ~((1<<29) | (1<<30));
	
	//Make them Open-Drain 
	AVR32_GPIO.port[0].odmers = 1;
	AVR32_GPIO.port[0].odmers = 1;
	
	//TWI configurations
	
	//TWI Clock Generator Register
	AVR32_TWI.CWGR.chdiv = 19;												
	AVR32_TWI.CWGR.cldiv = 19;		
	AVR32_TWI.CWGR.ckdiv = 0;
	
	
	/***************************
			SNAP COMMAND
	***************************/

	/*
	SENDING PART:
	This is a code for sending a command in general. 
	Edit variables cmd_size and cmd to change what you're sending
	*/
	
	int data_count=0;									//data counter variable
	int cmd_size = 13;									//Size of the command (no. of bytes)
	Byte cmd[] = {0x0A, 0x62, 0xC0, 0x00, 0x00, 0x02, 0x01, 'p', '1', '.', 'j', 'p', 'g'};
	
	//TWI Control Register
	AVR32_TWI.CR.msen = 1;												//Enable Master mode				
	AVR32_TWI.CR.svdis = 1;												//Disable Slave mode
	
	//TWI Master Mode Register
	AVR32_TWI.MMR.dadr = nanocam_addr;									//Device address of nanocam
	AVR32_TWI.MMR.mread = 0;											//Master write
	AVR32_TWI.MMR.iadrsz = 0;											//no internal address
	
	while( AVR32_TWI.SR.txcomp == 1){ 
					
		//Starts the Transfer
		AVR32_TWI.CR.start = 1;
		
		while(AVR32_TWI.SR.txrdy == 0)
			;//STFU and wait
		
		AVR32_TWI.THR.txdata = cmd[data_count];
		data_count++;
			
		while(data_count<cmd_size){
			
			while(AVR32_TWI.SR.txrdy == 0)
			;//STFU and wait
			
			AVR32_TWI.THR.txdata = cmd[data_count];
			data_count++;	
		}
		
		AVR32_TWI.CR.stop = 1;
		
	}
			
}

