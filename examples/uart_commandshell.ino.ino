#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BUFFER_SIZE 64
#define CMD_BUFFER_SIZE 32

// =====================
// UART RX RING BUFFER
// =====================
volatile unsigned char rx_buffer[BUFFER_SIZE];
volatile unsigned char head = 0;
volatile unsigned char tail = 0;

// =====================
// COMMAND BUFFER
// =====================
char cmd_buffer[CMD_BUFFER_SIZE];
unsigned char cmd_index = 0;

// =====================
// UART INIT
// =====================
void UART_Init(unsigned long baud)
{
    unsigned int ubrr = (F_CPU / (16UL * baud)) - 1;

    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    sei();  // Enable global interrupts
}

// =====================
// UART TRANSMIT BYTE
// =====================
void UART_Transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

// =====================
// UART SEND STRING
// =====================
void UART_SendString(const char *str)
{
    while (*str)
    {
        UART_Transmit(*str++);
    }
}

// =====================
// CHECK IF DATA AVAILABLE
// =====================
unsigned char UART_Available(void)
{
    return (head != tail);
}

// =====================
// READ FROM BUFFER
// =====================
unsigned char UART_Read(void)
{
    if (head == tail)
        return 0;

    unsigned char data = rx_buffer[tail];
    tail = (tail + 1) % BUFFER_SIZE;
    return data;
}

// =====================
// RX INTERRUPT
// =====================
ISR(USART_RX_vect)
{
    unsigned char data = UDR0;
    unsigned char next = (head + 1) % BUFFER_SIZE;

    if (next != tail)  // If buffer not full
    {
        rx_buffer[head] = data;
        head = next;
    }
}

// =====================
// COMMAND PROCESSOR
// =====================
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

// =====================
// SETUP
// =====================
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