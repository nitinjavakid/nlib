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

#include <adc.h>
#include <avr/io.h>

void n_adc_enable_ex(double max_adc_speed, double cpu)
{
    int prescalar = 7;
    int dividend = cpu/max_adc_speed;

    while((1 << prescalar) > dividend)
    {
        --prescalar;
    }

    ADCSRA = (1 << ADEN) | ((prescalar + 1) & 0x7);
}

void n_adc_disable()
{
    ADCSRA &= ~(1 << ADEN);
}

void n_adc_begin_read(n_adc_pin_t pin)
{
    ADMUX = (ADMUX & 0xf0) | (pin & 0x0f);
    ADCSRA |= (1 << ADSC);
}

int n_adc_in_progress()
{
    return ADCSRA & (1 << ADSC);
}

int n_adc_end_read()
{
    return ADC;
}

int n_adc_read(n_adc_pin_t pin)
{
    n_adc_begin_read(pin);
    while(n_adc_in_progress());
    return n_adc_end_read();
}

void n_adc_set_ref(n_adc_ref_t ref)
{
    ADMUX = (ref << 6) | (ADMUX & 0x3f);
}

n_adc_ref_t n_adc_get_ref()
{
    return (n_adc_ref_t)((ADMUX >> 6) & 0x3);
}
