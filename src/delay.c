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

#include <delay.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>
#include <math.h>

volatile static int count = 0;
static uint32_t fcpu = 0;
ISR(WDT_vect)
{
    --count;
    wdt_disable();
}

void n_delay_wait(int seconds, n_delay_sleep_mode_t mode)
{
    count = seconds/8;

    while(count > 0)
    {
        wdt_enable(WDTO_8S);
        WDTCSR |= (1 << WDIE);
        set_sleep_mode(mode);
        cli();
        sleep_enable();
        sleep_bod_disable();
        sei();
        sleep_cpu();
        sleep_disable();
        sei();
    }
}

void n_delay_sleep(n_delay_sleep_mode_t mode)
{
    set_sleep_mode(mode);
    cli();
    sleep_enable();
    sleep_bod_disable();
    sei();
    sleep_cpu();
    sleep_disable();
    sei();
}

void n_delay_loop(uint32_t ms)
{
    uint16_t onems = round(fcpu/1000000) * round(65535 / 262);
    while(ms)
    {
        _delay_loop_2(onems);
        --ms;
    }
}

void n_delay_init(uint32_t f)
{
    SMCR |= 0x01;
    fcpu = f;
}
