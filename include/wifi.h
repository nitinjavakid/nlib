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

#ifndef N_WIFI_H
#define N_WIFI_H

#include "io.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void* n_wifi_t;

    typedef enum _n_wifi_mode
    {
        N_WIFI_MODE_STA,
        N_WIFI_MODE_AP,
        N_WIFI_MODE_BOTH
    } n_wifi_mode_t;

    typedef enum _n_wifi_channel_type
    {
        N_WIFI_CHANNEL_TYPE_TCP,
        N_WIFI_CHANNEL_TYPE_UDP
    } n_wifi_channel_type_t;

    int n_wifi_status(n_wifi_t);
    int n_wifi_restart(n_wifi_t);
    int n_wifi_set_mode(n_wifi_t, n_wifi_mode_t);
    int n_wifi_connect(n_wifi_t, const char *endpoint, const char *password);

    n_io_handle_t n_wifi_new_io(n_wifi_t, const char *ipaddress, int port);

#ifdef __cplusplus
}
#endif

#endif
