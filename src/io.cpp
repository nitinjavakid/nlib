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

#include <io.h>
#include <io_impl.h>
#include <stdarg.h>
#include <stdio.h>

extern "C"
{
    char n_io_getch(n_io_handle_t handle)
    {
        if(!handle)
        {
            return -1;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        return impl->read();
    }

    void n_io_putch(n_io_handle_t handle, char val)
    {
        if(!handle)
        {
            return;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        impl->write(val);
    }

    size_t n_io_read(n_io_handle_t handle, void *buffer, size_t size)
    {
        if(!handle)
        {
            return -1;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        return impl->read(buffer, size);
    }

    size_t n_io_write(n_io_handle_t handle, const void *buffer, size_t size)
    {
        if(!handle)
        {
            return -1;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        return impl->write(buffer, size);
    }

    void n_io_vsprintf(n_io_handle_t handle, const char *fmt, va_list ap)
    {
        char *str = NULL;
        int len = 0;
        len = vsnprintf(NULL, 0, fmt, ap);
        str = (char *) calloc(len + 1, 1);
        len = vsnprintf(str, len + 1, fmt, ap);
        n_io_write(handle, str, len + 1);
        free(str);
    }

    void n_io_printf(n_io_handle_t handle, const char *fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);
        n_io_vsprintf(handle, fmt, ap);
        va_end(ap);
    }

    char *n_io_readline(n_io_handle_t handle, char *buffer, size_t size)
    {
        if(!handle)
        {
            return NULL;
        }

        bool internal = false;
        if(buffer == NULL)
        {
            internal = true;
            buffer = (char *) malloc(50);
            size = 50;
        }

        int idx = 0;
        char ch;

        while((ch = n_io_getch(handle)) != '\n')
        {
            buffer[idx++] = ch;

            if(idx == size - 1)
            {
                if(internal)
                {
                    char *rbuffer = (char *) realloc(buffer, size + 10);
                    if(rbuffer == NULL)
                    {
                        break;
                    }
                    buffer = rbuffer;
                    size += 10;
                }
                else
                {
                    break;
                }
            }
        }

        if(idx < size - 1)
        {
            buffer[idx++] = '\n';
        }

        buffer[idx] = '\0';
        return buffer;
    }

    int n_io_close(n_io_handle_t handle)
    {
        if(!handle)
        {
            return -1;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        int retval = impl->close();
        delete impl;
        return retval;
    }

    int n_io_on_recv(n_io_handle_t handle, void (*function)(int ch, void *), void *data)
    {
        if(!handle)
        {
            return -1;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        return impl->on_recv(function, data);
    }
}
