Interrupt-Driven UART Command Shell (ATmega328P)
Overview

This project implements a bare-metal UART driver for ATmega328P using interrupt-driven reception and circular buffering. A command parser interprets user input to control GPIO peripherals.

Features

Register-level UART configuration

RX interrupt-based data handling

Circular (ring) buffer implementation

Non-blocking main loop

Command-based LED control

Tested on Arduino UNO (16 MHz)