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

#ifndef N_IO_H
#define N_IO_H

#include <stdlib.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void* n_io_handle_t;

    char   n_io_getch(n_io_handle_t);
    void   n_io_putch(n_io_handle_t, char byte);

    size_t n_io_read(n_io_handle_t, void *buffer, size_t size);
    size_t n_io_write(n_io_handle_t, const void *buffer, size_t size);
    void   n_io_printf(n_io_handle_t, const char *fmt, ...);
    void   n_io_vsprintf(n_io_handle_t, const char *fmt, va_list ap);
    char  *n_io_readline(n_io_handle_t, char *buffer, size_t size);
    int    n_io_close(n_io_handle_t);
    int    n_io_on_recv(n_io_handle_t, void (*)(int ch, void *), void *data);

#ifdef __cplusplus
}
#endif

#endif
