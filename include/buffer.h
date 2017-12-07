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

#ifndef N_BUFFER_H
#define N_BUFFER_H

#include <debug.h>
#include <avr/interrupt.h>

class Buffer
{
public:
    Buffer(size_t bufsize)
        : size(bufsize)
    {
        buffer = (char *) malloc(size * sizeof(char));
        ridx = 0;
        widx = 0;
        datacount = 0;
    }

    void putch(int byte)
    {
        if(datacount == size)
        {
            return;
        }

        cli();
        buffer[widx] = byte;
        widx = (widx + 1) % size;
        ++datacount;
        sei();
    }

    size_t available()
    {
        return datacount;
    }

    int getch()
    {
        cli();
        char retval = buffer[ridx];
        ridx = (ridx + 1) % size;
        --datacount;
        sei();
        return retval;
    }

    ~Buffer()
    {
        free(buffer);
    }

private:
    char    *buffer;
    volatile size_t size;
    volatile size_t widx;
    volatile size_t ridx;
    volatile size_t datacount;
};

#endif
