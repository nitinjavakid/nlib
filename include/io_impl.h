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

#ifndef N_IO_IMPL_H
#define N_IO_IMPL_H

class IOImpl
{
public:
    virtual char   read() = 0;
    virtual void   write(char) = 0;
    virtual size_t read(void *buffer, size_t size) = 0;
    virtual size_t write(const void *buffer, size_t size) = 0;
    virtual int    close() = 0;
    virtual int    on_recv(void (*)(int, void *), void *) = 0;
    virtual ~IOImpl()
    {
    }
};

#endif
