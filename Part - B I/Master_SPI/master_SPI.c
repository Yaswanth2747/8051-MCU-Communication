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

void lcd_write_int(unsigned char num);  // Custom int print

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
    while (!(SPSTA & 0x80));  // wait for transfer complete
    return SPDAT;
}

void main(void) {
    unsigned char response;
    unsigned char prime1 = 7, prime2 = 13;

    lcd_init();
    master_greet_lcd();

    SPCON = 0x7F;  // Master mode, Fclk/16, CPOL=1, CPHA=1, SSDIS=1
    msdelay(500);

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_write_string("Sending Primes");

    msdelay(1000);
    spi_send(prime1);
    msdelay(1000);
    spi_send(prime2);
    msdelay(1000);

    lcd_cmd(0xC0);
    lcd_write_string("Waiting Result");

    msdelay(1500);  // Wait so slave has time to send response

    response = spi_send(0x00);  // Dummy write to receive

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    if (response == 1) {
        lcd_write_string("Sum is Prime");
    } else if (response == 2) {
        lcd_write_string("Sum Not Prime");
    } else {
        lcd_write_string("Unexpected Val");
        lcd_cmd(0xC0);
        lcd_write_char(response+'0');
    }

    while (1);
}
