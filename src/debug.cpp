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

#include <debug.h>
#include <stdarg.h>
#include <avr/interrupt.h>

static volatile n_io_handle_t handle = NULL;

extern "C"
{
    void n_debug_init(n_io_handle_t hdl)
    {
        handle = hdl;
    }

    void n_debug_printf(const PROGMEM char *fmt, ...)
    {
        if(handle == NULL) return;

        char *fmtram = (char *) malloc(strlen_P(fmt) + 1);
        memcpy_P(fmtram, fmt, strlen_P(fmt) + 1);
        va_list va;
        va_start(va, fmt);
        n_io_vsprintf(handle, fmtram, va);
        va_end(va);
        free(fmtram);
        n_io_printf(handle, "\r\n");
    }

    void n_debug_vprintf(const char *fmt, va_list va)
    {
        if(handle == NULL) return;

        n_io_vsprintf(handle, fmt, va);
        n_io_printf(handle, "\r\n");
    }
}
