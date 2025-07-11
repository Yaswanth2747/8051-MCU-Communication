#include <at89c5131.h>

bit tx_complete = 0, rx_complete = 0; // Bit flags for interrupts

/**********************************************************
uart_init(): 
	Initialization function to be completed
	Initializes UART peripheral for 8-bit transfer, 
	1 start and 1 stop bit. 
	
	Please write TH1 value for required baud rate
***********************************************************/	

void uart_init(void)
{
    TMOD = 0x20;               // Set Timer 1 Mode 2 (8-bit auto-reload) without affecting Timer 0, remember commented in main.c
    TH1 = 0xCC;                 // Load TH1 for 1200 bps with 24MHz crystal (TH1 = 256 - (Crystal_Freq / (384 * BaudRate)))
    SCON = 0x50;                // Configure UART for 8-bit data transfer
    TR1 = 1;                    // Start Timer 1
    ES = 1;                     // Enable Serial Interrupt
    EA = 1;                     // Enable Global Interrupt
}


/**********************************************************
transmit_char(<unsigned char ch>): 
	Transmits a character using UART
***********************************************************/	
void transmit_char(unsigned char ch)
{
	SBUF = ch;				        // Load data in SBUF
	while(!tx_complete); 			// Wait for tx_complete flag (interrupt to complete)
	tx_complete = 0;		      // Clear tx_complete flag 
}

/**********************************************************
transmit_string(<String pointer>): 
	Transmit a string using UART
***********************************************************/	
void transmit_string(unsigned char *s)
{
	while(*s != 0)
	{
			transmit_char(*s++);
	}
}

/**********************************************************
receive_char(): 
	Receives a character through UART. Returns a 
	character.
***********************************************************/		
unsigned char receive_char(void)
{
	unsigned char ch = 0;
	while(!rx_complete);				// Wait for rx_complete(interrupt to complete)
	rx_complete = 0;
	ch = SBUF;					        // Read data from SBUF
	return ch;					        // Return read character
}

/**********************************************************
Serial_ISR(): 
	Interrupt service routine for UART interrupt.
	Determines whether it is a transmit or receive
	interrupt and raise corresponding flag.
	Transmit or receive functions (defined above) monitor
	for these flags to check if data transfer is done.
***********************************************************/	
void serial_ISR(void) interrupt 4
{
		if(TI==1)			// Check whether TI is set
		{
			TI = 0;			// Clear TI flag
			tx_complete = 1;	// Set tx_complete flag indicating interrupt completion
		}
		else if(RI==1)			// Check whether RI is set
		{
			RI = 0;			// Clear RI flag
			rx_complete = 1;	// Set rx_complete flag indicating interrupt completion
		}
}
