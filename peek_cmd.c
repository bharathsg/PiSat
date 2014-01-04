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

struct list_reply{
	Byte csp_header[4];
	Byte status;
	Byte base_addr[4];
	Byte img_type[2];
	Byte img_size[4];
	Byte junk[5];
	} reply_components;

int i;

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
			PEEK COMMAND
	***************************/

	/*
	This assumes that the list command has been executed before this.
	Hence, reply_components must be filled up with meaningful information.
	DO: WAIT RA MACHA FLAG
	*/

	/*
	SENDING PART:
	This is a code for sending a command in general. 
	Edit variables cmd_size and cmd to change what you're sending
	*/
	
	static int num_chunks = 0;							//Nothing has been sent intially
	Byte chunk_base_addr[4];
	const int chunk_size = 120;
	chunk_base_addr[i] = reply_components.base_addr[i] + num_chunks*chunk_size;
	
	int data_count=0;									//data counter variable
	int cmd_size = 131;									//Size of the command (no. of bytes)
	//RE CHECK THIS COMMAND
	Byte cmd[131] = {0x0A, 0x60, 0x00, 0x00, 0x00, chunk_base_addr[0], chunk_base_addr[1], chunk_base_addr[2], chunk_base_addr[3], 0x78};
	
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
	
	/*
	RECIEVING PART
	Specify number of bytes expected to be receive.
	Received data is stored in 'reply'
	*/
	
	//CHECK THIS
	int data_size = 131;					//Expect to receive 20 bytes of data
	Byte reply[data_size];				//Array to hold received data
	
	AVR32_TWI.CR.msdis = 1;
	AVR32_TWI.CR.sven = 1;
	
	data_count = 0;
	
	while(data_count<data_size){
		
		if(AVR32_TWI.SR.svacc == 1)
			if(AVR32_TWI.SR.gacc == 0)
				if(AVR32_TWI.SR.svread == 0)
					if(AVR32_TWI.SR.rxrdy == 0){

						reply[data_count] = AVR32_TWI.RHR.rxdata;
						data_count++;

					}
	}
	
	num_chunks++;	
}