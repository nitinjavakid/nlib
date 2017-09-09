#ifndef N_USART_H
#define N_USART_H

#include "config.h"

#define n_usart_enable(x, y) n_usart_enable_ex(x, y, F_CPU)

void    n_usart_enable_ex(n_usart_bits_t bits, int baudrate, double cpuspeed);
void    n_usart_disable();
void    n_usart_write(uint8_t byte);
uint8_t n_usart_read();

#endif
