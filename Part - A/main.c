/**********************************************************
EE337: ADC IC MCP3008 interfacing with pt-51
(1) Complete spi_init() function from spi.h 

***********************************************************/

#include <at89c5131.h>
#include "lcd.h"																				//Driver for interfacing lcd 
#include "mcp3008.h"																		//Driver for interfacing ADC ic MCP3008


char adc_ip_data_ascii[6]={0,0,0,0,0,'\0'};							//string array for saving ascii of input sample
code unsigned char display_msg1[]="Volt.: ";						//Display msg on 1st line of lcd
code unsigned char display_msg2[]=" mV";								//Display msg on 2nd line of lcd



void main(void)
{
	int j=0;
	unsigned int adc_data=0;
	
	spi_init();																					
	adc_init();
    lcd_init();
	
	
	
	while(1)
	{
	 	unsigned int x,y,temp_data, pot_data;
		unsigned char str_data[6];
		
		x = adc(4);  // Read analog value from 4th channel of ADC MCP3008
		pot_data = (unsigned int) (x * 3.2258);  // Convert to millivolts

		y = adc(3);  // Read analog value from 3rd channel of ADC MCP3008
		temp_data = (unsigned int) (y * 3.2258) / 10;  // Convert to temperature (°C)

		// Display "Volt.: " on the first line of LCD
		lcd_cmd(0x80);  // Move cursor to first line
		lcd_write_string(display_msg1);

		// Convert pot_data to string (5-digit format) and print it
		int_to_string(pot_data, str_data);
		lcd_write_string(str_data);
		lcd_write_string(display_msg2);  // " mV"

		// Move cursor to the second line
		lcd_cmd(0xC0);  // Move cursor to second line
		lcd_write_string("Temp.: ");

		// Convert temp_data to string and print it
		int_to_string(temp_data, str_data);
		lcd_write_string(str_data);
		lcd_write_string(" oC");
		
	}

}

