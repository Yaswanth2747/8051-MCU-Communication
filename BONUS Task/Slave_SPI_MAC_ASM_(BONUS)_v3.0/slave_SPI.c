/*
 * SPI Slave Code with MAC integration v2.0
 * 
 * This file contains the implementation of the SPI Slave, developed as part of the EE337 - Microprocessors Laboratory course 
 * during the academic year 2024–25 at the Indian Institute of Technology Bombay.
 * also find the mac.asm file used to make the MAC CALL.
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

extern void MAC(void);
unsigned char *addr_70 = (unsigned char *)0x70; // a1
unsigned char *addr_71 = (unsigned char *)0x71; // a2
unsigned char *addr_72 = (unsigned char *)0x72; // a3
unsigned char *addr_73 = (unsigned char *)0x73; // b1
unsigned char *addr_74 = (unsigned char *)0x74; // b2
unsigned char *addr_75 = (unsigned char *)0x75; // b3

unsigned char *addr_52 = (unsigned char *)0x52; // final MAC sum
unsigned int sum_asm;

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

unsigned char values[6];  // stores a1, a2, a3, b1, b2, b3
unsigned char index = 0;

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
    unsigned char buffer[6] = {0, 0, 0, 0, 0, '\0'};

    lcd_init();
    slave_greet_lcd();

    SPCON = 0x6F;  // Slave mode, Fclk/16, CPOL=1, CPHA=1, SSDIS=1
    msdelay(500);

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_write_string("Waiting Data");

    while (1) {
        while (!(SPSTA & 0x80));  // Wait for transfer complete
        received = SPDAT;

        values[index] = received;

        lcd_cmd(0x01);
        lcd_cmd(0x80);
        lcd_write_string("Got Data:");
        lcd_cmd(0xC0);
        int_to_string(received, buffer);
        lcd_write_string(buffer);
        msdelay(1000);

        index++;

        if (index == 6) {
            // Assign values to memory locations
            *addr_70 = values[0]; // a1
            *addr_71 = values[1]; // a2
            *addr_72 = values[2]; // a3
            *addr_73 = values[3]; // b1
            *addr_74 = values[4]; // b2
            *addr_75 = values[5]; // b3

            MAC();  // Assembly function

            sum_asm = *addr_52;  // Neglect upper nibble
            sum = (unsigned char)sum_asm;

            lcd_cmd(0x01);
            lcd_cmd(0x80);
            lcd_write_string("MAC Sum:");
            int_to_string(sum, buffer);
            lcd_write_string(buffer);
            msdelay(1000);

            if (is_prime(sum)) {
                SPDAT = 1;  // Sum is prime
                lcd_cmd(0xC0);
                lcd_write_string("Sum is Prime");
            } else {
                SPDAT = 2;  // Not prime
                lcd_cmd(0xC0);
                lcd_write_string("Not Prime");
            }

            while (1);  // Halt further reception
        }
    }
}
