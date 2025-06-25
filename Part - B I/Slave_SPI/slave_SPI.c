/*
 * SPI Slave Code
 * 
 * This file contains the implementation of the SPI Slave, developed as part of the EE337 - Microprocessors Laboratory course 
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

void delay() {
    int i;
    for(i = 0; i < 5000; i++);
}

bit is_prime(unsigned char num) {
    unsigned char i;
    if (num < 2) return 0;
    for (i = 2; i <= num / 2; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

unsigned char prime1 = 0, prime2 = 0;
bit received_first = 0;

void slave_greet_lcd() {
    msdelay(10);
    lcd_cmd(0x80);
    msdelay(4);
    lcd_write_string("SPI Slave");

    lcd_cmd(0xC0);
    msdelay(4);
    lcd_write_string("Waiting");
    msdelay(500);
    lcd_write_char('.');
    msdelay(500);
    lcd_write_char('.');
    msdelay(500);
    lcd_write_char('.');
    msdelay(500);
}


void main() {
    unsigned char received;
    unsigned char sum;
	unsigned char buffer[6]={0,0,0,0,0,'\0'};
    lcd_init();
    slave_greet_lcd();

    SPCON = 0x6F;  // Slave mode, Fclk/16, CPOL=1, CPHA=1, SSDIS=1
    msdelay(500);

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_write_string("Waiting Data");

    while (1) {
        while (!(SPSTA & 0x80));  // wait for transfer complete
        received = SPDAT;

        if (!received_first) {
            prime1 = received;
            received_first = 1;

            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_write_string("Got Prime1:");
            lcd_cmd(0xC0);
					int_to_string(prime1,buffer);
            lcd_write_string(buffer);
        } else {
            prime2 = received;

            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_write_string("Got Prime2:");
            lcd_cmd(0xC0);
					int_to_string(prime2,buffer);
            lcd_write_string(buffer);

            msdelay(1000);

            sum = prime1 + prime2;
            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_write_string("Sum:");
					int_to_string(sum,buffer);
            lcd_write_string(buffer);

            msdelay(1000);

            if (is_prime(sum)) {
                SPDAT = 1;  // Prime
                lcd_cmd(0xC0);
                lcd_write_string("Sum is Prime");
								while(1);
            } else {
                SPDAT = 2;  // Not prime
                lcd_cmd(0xC0);
                lcd_write_string("Not Prime");
								while(1);
						}

            msdelay(1000);  // Allow master to read SPDAT before loop repeats

            received_first = 0;
            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_write_string("Waiting Data");
        }
    }
}