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

#ifndef N_USART_H
#define N_USART_H

#include "config.h"
#include "io.h"

#define n_usart_enable(bits, parity, baud, stopbit) n_usart_enable_ex(bits, parity, baud, stopbit, F_CPU)

#define n_usart_set_interrupt_flag(rx, tx) \
    do {                                                        \
        UCSR0B = (UCSR0B & ~((1 << RXCIE0) | (1 << TXCIE0))) |  \
            ((rx & 0x1) << RXCIE0) | ((tx & 0x1) << TXCIE0);    \
    } while(0)

#ifdef __cplusplus
extern "C"
{
#endif

    void    n_usart_enable_ex(n_usart_bits_t bits, n_usart_parity_t parity, n_usart_stopbit_t stopbit, uint32_t baudrate, double cpuspeed);

    void    n_usart_disable();
    void    n_usart_write(uint8_t byte);
    uint8_t n_usart_read();

    n_io_handle_t n_usart_new_io(size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif
