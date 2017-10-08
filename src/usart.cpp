#include <usart.h>
#include <io_impl.h>

class USARTIo : public IOImpl
{
public:
    USARTIo()
    {
    }

    char read()
    {
        return n_usart_read();
    }

    void write(char val)
    {
        n_usart_write(val);
    }

    int read(void *buffer, size_t size)
    {
        char *chbuffer = (char *) buffer;
        for(size_t i=0; i < size; i++)
        {
            chbuffer[i] = n_usart_read();
        }
        return size;
    }

    int write(const void *buffer, size_t size)
    {
        const char *chbuffer = (char *) buffer;
        for(size_t i=0; i < size; i++)
        {
            n_usart_write(chbuffer[i]);
        }
        return size;
    }

    int close()
    {
    }

    ~USARTIo()
    {
    }
};

extern "C"
{
    void n_usart_enable_ex(n_usart_bits_t bits, n_usart_parity_t parity, n_usart_stopbit_t stopbit, int baudrate, double cpuspeed)
    {
        UBRR0 = (cpuspeed / (baudrate * 16UL)) - 1;

        UCSR0C = ((bits & 0x3) << 1) | ((parity & 0x3) << 4) | ((stopbit & 0x1) << 3);

        UCSR0B = (1 << RXEN0) | (1<<TXEN0) | (bits & 0x4);
    }

    void n_usart_disable()
    {
        UCSR0B &= ~((1<<RXEN0)|(1<<TXEN0));
    }

    void n_usart_write(uint8_t byte)
    {
        while(!(UCSR0A&(1<<UDRE0)));
        UDR0 = byte;
        while(!(UCSR0A&(1<<TXC0)));
        while(!(UCSR0A&(1<<UDRE0)));
    }

    uint8_t n_usart_read()
    {
        while(!(UCSR0A&(1<<RXC0)));
        return UDR0;
    }

    n_io_handle_t n_usart_new_io()
    {
        return new USARTIo();
    }
}
