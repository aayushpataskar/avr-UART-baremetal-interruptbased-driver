#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_Init(uint32_t baud);
void UART_Transmit(uint8_t data);
void UART_SendString(const char *str);
uint8_t UART_Available(void);
uint8_t UART_Read(void);

#endif