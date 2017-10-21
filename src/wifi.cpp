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

#include <wifi.h>
#include <wifi_impl.h>

extern "C"
{
    int n_wifi_status(n_wifi_t hnd)
    {
        if(!hnd)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->status();
    }

    int n_wifi_restart(n_wifi_t hnd)
    {
        if(!hnd)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->restart();
    }

    int n_wifi_set_mode(n_wifi_t hnd, n_wifi_mode_t mode)
    {
        if(!hnd)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->set_mode(mode);
    }

    int n_wifi_get_mode(n_wifi_t hnd, n_wifi_mode_t *mode)
    {
        if(!hnd || !mode)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->get_mode(mode);
    }

    int n_wifi_connect(n_wifi_t hnd, const char *endpoint, const char *password)
    {
        if(!hnd)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->connect(endpoint, password);
    }

    int n_wifi_set_network(n_wifi_t hnd, const char *ip, const char *gateway, const char *netmask)
    {
        if(!hnd)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->set_network(ip, gateway, netmask);
    }

    n_io_handle_t n_wifi_open_io(n_wifi_t hnd, n_wifi_io_type_t type, const char *ipaddress, int port, size_t buffer_size)
    {
        if(!hnd)
        {
            return NULL;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->open_io(type, ipaddress, port, buffer_size);
    }

    int n_wifi_close(n_wifi_t hnd)
    {
        if(!hnd)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        int retval = impl->close();
        delete impl;
        return retval;
    }

    int n_wifi_get_ap_nodes(n_wifi_t hnd, n_wifi_ap_node_t **nodes)
    {
        if(!hnd)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->get_ap_nodes(nodes);
    }

    int n_wifi_free_ap_nodes(n_wifi_t hnd, n_wifi_ap_node_t *nodes)
    {
        if(!hnd)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->free_ap_nodes(nodes);
    }
}
