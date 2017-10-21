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

#ifndef N_DELAY_H
#define N_DELAY_H

#include <config.h>


#ifdef __cplusplus
extern "C"
{
#endif

    void n_delay_init(uint32_t fcpu);
    void n_delay_wait(int seconds, n_delay_sleep_mode_t mode);
    void n_delay_sleep(n_delay_sleep_mode_t mode);
    void n_delay_loop(uint32_t milliseconds);

#ifdef __cplusplus
}
#endif

#endif
