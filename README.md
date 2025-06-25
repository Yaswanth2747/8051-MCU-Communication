# 8051-Based Microprocessors Lab Project – EE337 (Spring 2025)

> SPI & UART Communication | Sensor Interfacing | Assembly Integration  
> Department of Electrical Engineering, IIT Bombay  
> Instructor: **Prof. Sachin Patkar**

<p align="center">
  <img src="./Part - A/implementation.png" alt="Part A Implementation" width="500"/>
</p>

## 🔧 Platform Details

This project was implemented using **PT51 development boards** designed by **WEL Labs, IIT Bombay**.  
These boards are based on the **AT89C5131A**, an enhanced 8051-compatible microcontroller from Atmel with built-in USB, SPI, UART, and ADC support.

We used **Keil µVision** for code development and **RealTerm** for UART-based serial communication with the host PC.

---

## 📁 Project Structure

This repository contains the complete 3-week lab project, structured into four phases:

```

.
├── Part - A/           # SPI-based ADC interface (MCP3008) + LM35 sensor + LCD display
├── Part - B I/         # SPI Master-Slave board-to-board communication: Prime sum check
├── Part - B II/        # UART input → SPI processing → UART output + Assembly routine
├── BONUS/             # MAC operation using optimized assembly
└── README.md

```

---

## Summary of Work

### 🔹 Part A – SPI ADC & Temperature Display
- Interfaced the **MCP3008 ADC** with the 8051 MCU via **SPI**
- Verified with voltage divider, then integrated **LM35 sensor** for ambient temperature
- Displayed readings on **16x2 LCD**

---

### 🔹 Part B I – Board-to-Board Communication (SPI)
- Master sends two prime numbers via **SPI**
- Slave computes sum and checks for primality
- Result displayed on both boards’ LCDs

---

### 🔹 Part B II – UART ↔ SPI Pipeline + Assembly
- Inputs received from PC using **UART** (via **RealTerm**)
- Forwarded to Slave over **SPI**
- Assembly routine used for arithmetic operation
- Response sent back to PC via **UART**

---

### 🔹 Bonus – MAC Operation
- Replaced addition with **Multiply-Accumulate (MAC)** using optimized **assembly code**

---

## Tools & Technologies

- **AT89C5131A Microcontroller (8051 Core)**
- **PT51 Development Boards – WEL Labs, IIT Bombay**
- **Keil µVision** for Embedded C + Assembly
- **RealTerm** and **TeraTerm** for UART-based serial interaction
- **MCP3008**, **LM35**, 16x2 **LCD**

---

## 📁 Each Folder Includes

- Respective Problem Statement
- Source code (`.c`, `.asm`, etc.)
- Report
