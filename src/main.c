#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BUFFER_SIZE 64
#define CMD_BUFFER_SIZE 32


void setup()
{
    DDRB |= (1 << PB5);  // Pin 13 output (LED)
    UART_Init(9600);
    UART_SendString("UART Command Interface Ready\r\n");
}

// =====================
// LOOP
// =====================
void loop()
{
    if (UART_Available())
    {
        char c = UART_Read();

        // Echo character
        UART_Transmit(c);

        if (c == '\r' || c == '\n')
        {
            UART_SendString("\r\n");
            cmd_buffer[cmd_index] = '\0';
            process_command(cmd_buffer);
            cmd_index = 0;
        }
        else
        {
            if (cmd_index < CMD_BUFFER_SIZE - 1)
            {
                cmd_buffer[cmd_index++] = c;
            }
        }
    }
}