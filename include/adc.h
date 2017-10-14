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


#ifndef N_ADC_H
#define N_ADC_H

#include "config.h"

#define     n_adc_enable(x) n_adc_enable_ex(x, F_CPU)

#ifdef __cplusplus
extern "C"
{
#endif

void        n_adc_disable();
void        n_adc_begin_read(n_adc_pin_t pin);
int         n_adc_in_progress();
int         n_adc_end_read();
int         n_adc_read(n_adc_pin_t pin);
void        n_adc_set_ref(n_adc_ref_t ref);
n_adc_ref_t n_adc_get_ref();

// Internal functions
void        n_adc_enable_ex(double prescalar, double cpu);

#ifdef __cplusplus
}
#endif

#endif
