#ifndef N_USART_H
#define N_USART_H

#include "config.h"

#define n_usart_enable(bits, parity, baud, stopbit) n_usart_enable_ex(bits, parity, baud, stopbit, F_CPU)

void    n_usart_enable_ex(n_usart_bits_t bits, n_usart_parity_t parity, n_usart_stopbit_t stopbit, int baudrate, double cpuspeed);

#define n_usart_set_interrupt_flag(rx, tx) \
    do {                                                        \
        UCSR0B = (UCSR0B & ~((1 << RXCIE0) | (1 << TXCIE0))) |  \
            ((rx & 0x1) << RXCIE0) | ((tx & 0x1) << TXCIE0);    \
    } while(0)

void    n_usart_disable();
void    n_usart_write(uint8_t byte);
uint8_t n_usart_read();

#endif
