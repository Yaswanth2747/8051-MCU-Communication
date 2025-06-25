/**********************************************************
This header file contains function related uC 8051 SPI module. 
It has functions to initialize spi module and to communicate with SPI slave 
***********************************************************/

//preprocessor directives for guarding against multiple inclusion of header files, read compiler user guide for more information

#ifndef spi_h																						//if identifier spi_h is not defined then goto next line
	#define spi_h 1																				//Define spi_h identifier and assign constant 1 to it 
#endif																									//end if directive

//Declaration of functions which are contained in this header file

void spi_init(void);																		//Initialize SPI module 
unsigned long int spi_trx(unsigned long int spi_data_tx);				//Takes 3 bytes as an input and send it to SPI slave IC and returns 3 bytes which are received from slave
void spi_interrupt(void);																//SPI ISR 																												


//Variable declaration

bit transmit_completed= 0;															//To check if SPI data transmit is complete
unsigned char temp_spi_data;														//Temporary variable

//Function definition


/**********************************************************
  spi_init(): 
	
	Initialization function to be completed
	Initializes SPI peripheral for interfacing with MCP3008, 
	Choose appropriate SPCON value by referring to 
	MCP3008 and AT89C5131 datasheets  	
***********************************************************/
void spi_init(void)
{
    /* From Datasheet https://ww1.microchip.com/downloads/en/DeviceDoc/doc4337.pdf
        SPCON Register Configuration (Hex: 0x3F)
        | Bit  | Name   | Description                                      | Value |
        |------|--------|--------------------------------------------------|-------|
		|  7   | SPR2   | Reserved (must be 0)                             |   0   |
        |  6   | SPEN   | 0 = SPI initially disabled (enabled later)       |   0   |
		|  5   | SSDIS  | 1 = Disable Slave Select (SSBAR), free P1.1      |   1   |
        |  4   | MSTR   | 1 = Set MCU as SPI Master                        |   1   |
        |  3   | CPOL   | 1 = SCK idle high                                |   1   |
        |  2   | CPHA   | 1 = Data sampled on the second edge (trailing)   |   1   |
        | 1-0  | SPR1:0 | 11 = SPI Clock = Fclk/16 (750 kHz @ 12 MHz)      |  11   |

        Binary Representation:  0b  0 0 1 1 | 1 1 1 1
        Hexadecimal Value:      0x     3    |    f
    */

    SPCON = 0x3f;  // Configure SPI Control Register 0x3F, Mode 3 SPI, idle state is HIGH and data sampled at rising edge.

    IEN1 |= 0x04;  // Enable SPI interrupt (ESPI = 1)    

    EA = 1;  // Enable global interrupts

    SPCON |= 0x40;  // Enable SPI module (SPEN = 1)
}
/**********************************************************
  spi_trx(): 
	
	Transmits 3 bytes to SPI slave and receive 3 bytes. 
	These 3 bytes are packed into 4 bytes as long datatype.
***********************************************************/
unsigned long int spi_trx(unsigned long int spi_data_tx)
{
	//declaration of local variables 
	unsigned long int spi_data_rx;
	unsigned char spi_data_3,spi_data_2,spi_data_1;
	
	//Send 3rd byte
		transmit_completed = 0;
		
		SPDAT = (spi_data_tx>>16)%256;											//Sending most significant byte of data to SPI slave 
	
		while(!transmit_completed);													//Wait till transmission is complete. 	
			spi_data_3	=	temp_spi_data ;											//Most significant byte will be received  
	
	//Send 2nd byte
		transmit_completed=0;
		SPDAT = (spi_data_tx>>8)%256;												//Extracting and sending byte between MS and LS byte to SPI slave	
		
		while(!transmit_completed);													//Wait till transmission is complete.
			spi_data_2	=	temp_spi_data ;											//Save byte received from SPI slave
	
	//Send 1st byte
		transmit_completed=0;
		SPDAT = (spi_data_tx%256);													//Extracting least significant byte and sending it to spi slave	
		
		while(!transmit_completed);													//Wait till transmission is complete.
			spi_data_1	=	temp_spi_data ;											//Save byte received from SPI slave
			
		spi_data_rx= (spi_data_3<<16) + (spi_data_2<<8) + spi_data_1;			//Concatenate all received byte to form a 32 bit word
	
		return spi_data_rx;																	//return it	
}

/**********************************************************
  spi_interrupt() 
	
	Interrupt service routine for SPI interrupt.
	Generates interrupt if transmission and reception are complete.
	Transmit and receive function (defined above) monitor
	for these flags to check if data transfer is done.
***********************************************************/
void spi_interrupt(void) interrupt 9 										// interrupt address is 0x004B, (Address -3)/8 = interrupt no.
{
	
	switch	( SPSTA & 0xf0)         											//read and clear SPI status register, for more info: table no 76 from datasheet 
	{
		case 0x80:																					//SPIF is set i.e. SP data transfer is complete 
			temp_spi_data=SPDAT;  														//Read received data 
			transmit_completed=1;															//Set transmission complete flag 
 		break;

		case 0x10:
			
		break;
	
		case 0x40:
		break;
	}
}
