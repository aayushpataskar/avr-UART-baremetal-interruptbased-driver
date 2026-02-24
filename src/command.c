#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BUFFER_SIZE 64
#define CMD_BUFFER_SIZE 32
void process_command(char *cmd)
{
    if (strcmp(cmd, "LED ON") == 0)
    {
        PORTB |= (1 << PB5);
        UART_SendString("LED TURNED ON\r\n");
    }
    else if (strcmp(cmd, "LED OFF") == 0)
    {
        PORTB &= ~(1 << PB5);
        UART_SendString("LED TURNED OFF\r\n");
    }
    else if (strcmp(cmd, "STATUS") == 0)
    {
        if (PORTB & (1 << PB5))
            UART_SendString("LED IS ON\r\n");
        else
            UART_SendString("LED IS OFF\r\n");
    }
    else if (strcmp(cmd, "HELP") == 0)
    {
        UART_SendString("Commands:\r\n");
        UART_SendString("LED ON\r\n");
        UART_SendString("LED OFF\r\n");
        UART_SendString("STATUS\r\n");
    }
    else
    {
        UART_SendString("UNKNOWN COMMAND\r\n");
    }
}