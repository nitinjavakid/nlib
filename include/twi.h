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

#ifndef N_TWI_H
#define N_TWI_H

#include "config.h"
#include "io.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // Not to be used, design not complete
    n_io_handle_t n_twi_new_slave_io(uint8_t address, size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif
