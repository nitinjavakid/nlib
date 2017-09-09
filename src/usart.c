#include <usart.h>

void n_usart_enable_ex(n_usart_bits_t bits, int baudrate, double cpuspeed)
{
    UBRR0 = (cpuspeed / baudrate * 16UL) - 1;

    UCSR0C |= (bits & 0x7) << 1;

    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

void n_usart_disable()
{
    UCSR0B &= ~((1<<RXEN0)|(1<<TXEN0));
}

void n_usart_write(uint8_t byte)
{
    while(!(UCSR0A&(1<<UDRE0)));
    UDR0 = byte;
}

uint8_t n_usart_read()
{
    while(!(UCSR0A&(1<<RXC0)));
    return UDR0;
}
