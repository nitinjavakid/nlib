#ifndef N_ADC_H
#define N_ADC_H

#include "config.h"

#define     n_adc_enable(x) n_adc_enable_ex(x, F_CPU)

void        n_adc_disable();
void        n_adc_begin_read(n_adc_pin_t pin);
int         n_adc_in_progress();
int         n_adc_end_read();
int         n_adc_read(n_adc_pin_t pin);
void        n_adc_set_ref(n_adc_ref_t ref);
n_adc_ref_t n_adc_get_ref();

// Internal functions
void        n_adc_enable_ex(double prescalar, double cpu);

#endif
