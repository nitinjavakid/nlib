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

#include <io_impl.h>
#include <twi.h>
#include <buffer.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <buffer.h>
#include <avr/power.h>
#include <delay.h>

static Buffer *buffer = NULL;
static void (*on_recv)(int, void *) = NULL;
static void *on_recv_data = NULL;

ISR(TWI_vect)
{
    uint8_t status = TWSR & 0xFC; // mask-out the prescaler bits
    switch(status)
    {
    case TW_SR_DATA_ACK: // data received, ACK returned
        int byte = TWDR;
        if(buffer)
        {
            buffer->putch(byte);
        }

        if(on_recv)
        {
            on_recv(byte, on_recv_data);
        }
        break;
    }

    TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
}

class TWISlaveIo : public IOImpl
{
private:
    uint8_t address;
public:
    TWISlaveIo(uint8_t addr, size_t buffer_size)
        : address(addr)
    {
        buffer = new Buffer(buffer_size);
    }

    char read()
    {
        char retval;
        cli();
        TWAR = address << 1;
        TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
        sei();

        while(!buffer->available())
        {
            power_twi_enable();
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
    }

    size_t read(void *buffer, size_t size)
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

    size_t write(const void *buffer, size_t size)
    {
        return -1;
    }

    int close()
    {
        cli();
        delete buffer;
        sei();
    }

    int on_recv(void (*func)(int, void *), void *data)
    {
        ::on_recv = func;
        on_recv_data = data;
    }

    ~TWISlaveIo()
    {
    }
};

extern "C"
{
    n_io_handle_t n_twi_new_slave_io(uint8_t address, size_t buffer_size)
    {
        return new TWISlaveIo(address, buffer_size);
    }
}
