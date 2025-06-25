/*
 * SPI Master Code
 * 
 * This file contains the implementation of the SPI Master, developed as part of the EE337 - Microprocessors Laboratory course 
 * during the academic year 2024–25 at the Indian Institute of Technology Bombay.
 * 
 * This code is intended solely for academic use.
 * 
 * Authors:
 * - Yaswanth Ram Kumar (Roll No: 23B1277)
 * - Pragyan Paramita Behera (Roll No: 23B1278)
 * 
 * Department of Electrical Engineering,  
 * Indian Institute of Technology Bombay,  
 * Powai, Mumbai - 400076, India.
 */

#include <at89c5131.h>
#include "lcd.h"
#include "serial.h"     // UART interfacing functions
#include <stdio.h>

// Read a number (max 3 digits) from UART input
unsigned char receive_num(void)
{
    char buffer[4];  // Max 3 digits + null terminator
    unsigned char i = 0;
    unsigned char num = 0;
    char ch;

    // Read characters until Enter is pressed
    while (1)
    {
        ch = receive_char();       // Get character from UART
        transmit_char(ch);         // Echo back

        if (ch == '\r')            // Enter key (Carriage Return)
        {
            buffer[i] = '\0';      // Null-terminate the string
            break;
        }

        // Accept only digits, and at most 3 digits
        if (ch >= '0' && ch <= '9' && i < 3)
        {
            buffer[i++] = ch;
        }
    }

    // Convert buffer to number
    for (i = 0; buffer[i] != '\0'; i++)
    {
        num = num * 10 + (buffer[i] - '0');
    }

    return num; // Will be in range 0–255
}

// Display welcome message on LCD
void master_greet_lcd() {
    msdelay(10);
    lcd_cmd(0x80);
    msdelay(4);
    lcd_write_string("SPI Master");

    lcd_cmd(0xC0);
    msdelay(4);
    lcd_write_string("Running");
    msdelay(500);
    lcd_write_char('.');
    msdelay(500);
    lcd_write_char('.');
    msdelay(500);
    lcd_write_char('.');
    msdelay(500);
}

// Send and receive 1 byte over SPI
unsigned char spi_send(unsigned char dat) {
    SPDAT = dat;
    while (!(SPSTA & 0x80));  // Wait for transfer complete
    return SPDAT;
}

void main(void) {
    unsigned char response;
    unsigned char nums[6];
    unsigned char i;

    /** Configuration For Serial Communication **/
    // TMOD = 0x01;  // This is written explicitly in serial.h
    ET0 = 1; // Enable Timer 0 interrupt
    EA = 1;  // Enable global interrupts
    // P1 = 0x0F;
    /*********************************************/

    uart_init();
    lcd_init();
    master_greet_lcd();

    transmit_string("Enter 3 values for A (a1, a2, a3):\r\n");
    for (i = 0; i < 3; i++) {
        transmit_string("Enter value: ");
        nums[i] = receive_num();
    }

    transmit_string("Enter 3 values for B (b1, b2, b3):\r\n");
    for (i = 3; i < 6; i++) {
        transmit_string("Enter value: ");
        nums[i] = receive_num();
    }

    SPCON = 0x7F;  // Master mode, Fclk/16, CPOL=1, CPHA=1, SSDIS=1
    msdelay(500);

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_write_string("Sending Data");

    for (i = 0; i < 6; i++) {
        msdelay(1000);
        spi_send(nums[i]);
    }

    lcd_cmd(0xC0);
    lcd_write_string("Waiting Result");

    msdelay(3500);  // Wait so slave has time to compute

    response = spi_send(0x00);  // Dummy write to receive

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    if (response == 1) {
        lcd_write_string("Sum is Prime");
				transmit_string("Sum is a prime number Sir.");

    } else if (response == 2) {
        lcd_write_string("Sum Not Prime");
				transmit_string("Sum is NOT a prime number Sir."); 
    } else {
        lcd_write_string("Unexpected Val");
        lcd_cmd(0xC0);
        lcd_write_char(response+'0');
				transmit_string("Unexpected Val");
    }
    while (1);
}
