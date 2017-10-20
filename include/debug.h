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

#ifndef N_DEBUG_H
#define N_DEBUG_H

#include <io.h>
#include <stdarg.h>

#ifdef DEBUG
#define N_DEBUG(fmt, ...) n_debug_printf(fmt, ##__VA_ARGS__)
#define N_VDEBUG(fmt, va) n_debug_vprintf(fmt, va)
#else
#define N_DEBUG(fmt, ...) do { } while(0)
#define N_VDEBUG(fmt, va) do { } while(0)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    void n_debug_printf(const char *fmt, ...);
    void n_debug_vprintf(const char *fmt, va_list);
    void n_debug_init(n_io_handle_t);

#ifdef __cplusplus
}
#endif

#endif
