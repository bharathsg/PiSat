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
#define DADR 0b0110100 //Factory programmed address: 0110100b 


#define IADR_start 0x01	//Internal address of the slave's memory which you want to read from
#define IADR_end 0x0F   //Need to check the data sheet of the battery monitor and put something here
					   //For now let us read only one thing from the battery monitor, battery voltage maybe?
					   //Also this would mean we need to read only ONE BYTE. (So i'm using that flowchart for reference)
	int i;									//Loop counter variable
	int8_t rcv_data[IADR_end];		//Starts from zero address

int main (void)
{
	board_init();
	

	
	/************************
		Pin configurations
	 ************************/
	
	//Make them Peripheral lines
	AVR32_GPIO.port[0].gper &= ~((1<<29) | (1<<30));			//Clears 29th & 30th bits (makes them peripheral pins)
	AVR32_GPIO.port[0].pmr0 &= ~((1<<29) | (1<<30));			//Sets them as I2C pins
	AVR32_GPIO.port[0].pmr1 &= ~((1<<29) | (1<<30));
	
	//Make them Open-Drain 
	AVR32_GPIO.port[0].odmers = 1;
	AVR32_GPIO.port[0].odmers = 1;
	
	/************************
		TWI configurations
	 ************************/

	//TWI Clock Generator Register
	AVR32_TWI.CWGR.chdiv = 19;												
	AVR32_TWI.CWGR.cldiv = 19;		
	AVR32_TWI.CWGR.ckdiv = 0;
	
	//TWI Control Register
	AVR32_TWI.CR.msen = 1;												//Enable Master mode				
	AVR32_TWI.CR.svdis = 1;												//Disable Slave mode
	
	//TWI Master Mode Register
	AVR32_TWI.MMR.dadr = DADR;											//Device address of Battery monitor
	AVR32_TWI.MMR.mread = 1;											//Transfer direction
	AVR32_TWI.MMR.iadrsz = 2;											//2-byte internal address
	
	//Set internal address
	AVR32_TWI.iadr = IADR_start;
	
	
	/***********************
		Data Transfer
	 ***********************/
	
	while( AVR32_TWI.SR.txcomp == 1){ 
		//for(int i = IADR_start; i<=IADR_end-1; i++){
			
			i = IADR_start;
			AVR32_TWI.CR.stop = 1;
			
			//Starts the Transfer
			AVR32_TWI.CR.start = 1;
			
			//Read status register and see if data is received
			while(! AVR32_TWI.SR.rxrdy )
					;//Wait till byte is received (RXRDY is set)
			
			rcv_data[i-1] = AVR32_TWI.RHR.rxdata;
			//}
		
		//Last data byte
		//i++;
		
		//AVR32_TWI.CR.stop = 1;
		//while(! AVR32_TWI.SR.rxrdy )
					;//Wait till byte is received (RXRDY is set)
		//rcv_data[i-1] = AVR32_TWI.RHR.rxdata;
	}
}

