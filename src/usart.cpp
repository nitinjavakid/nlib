/*
 * Copyright (c) 2016-2017 Contributors as noted in the AUTHORS file
 *
 * This file is part of nlib.
 *
 * nlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>
 */

#include <usart.h>
#include <io_impl.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <delay.h>
#include <math.h>
#include <buffer.h>

static Buffer *buffer = NULL;

ISR(USART_RX_vect)
{
    int byte = n_usart_read();

    if(buffer)
    {
        buffer->putch(byte);
    }
}

class USARTIo : public IOImpl
{
public:
    USARTIo(size_t buffer_size)
    {
        buffer = new Buffer(buffer_size);
        UCSR0B = (UCSR0B & (~(1 << RXCIE0))) | (1 << RXCIE0);
    }

    char read()
    {
        char retval;
        UCSR0B = (UCSR0B & (~(1 << RXCIE0))) | (1 << RXCIE0);
        sei();
        while(!buffer->available())
        {
            power_usart0_enable();
            n_delay_sleep(N_DELAY_IDLE);
        }

        cli();
        if(buffer->available())
        {
            retval = buffer->getch();
        }
        sei();
        return retval;
    }

    void write(char val)
    {
        n_usart_write(val);
    }

    int read(void *buffer, size_t size)
    {
        size_t idx = 0;
        char *chbuffer = (char *) buffer;
        while(idx < size)
        {
            chbuffer[idx] = read();
            --idx;
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
        cli();
        delete buffer;
        sei();
    }

    ~USARTIo()
    {
    }
};

extern "C"
{
    void n_usart_enable_ex(n_usart_bits_t bits, n_usart_parity_t parity, n_usart_stopbit_t stopbit, uint32_t baudrate, double cpuspeed)
    {
        UBRR0 = round((cpuspeed / (baudrate * 16UL)) - 1);

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

    n_io_handle_t n_usart_new_io(size_t buffer_size)
    {
        return new USARTIo(buffer_size);
    }
}
