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

#ifndef N_WIFI_IMPL_H
#define N_WIFI_IMPL_H

class WIFIImpl
{
public:
    virtual int status() = 0;
    virtual int restart() = 0;
    virtual int set_mode(n_wifi_mode_t) = 0;
    virtual int connect(const char *, const char *) = 0;
    virtual n_io_handle_t open_io(const char *, int) = 0;
    virtual int close() = 0;
    virtual ~WIFIImpl()
    {
    }
};

#endif
