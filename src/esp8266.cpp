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

#include "esp8266.h"
#include "wifi_impl.h"
#include "io.h"
#include <string.h>
#include <buffer.h>

class ESP8266Wifi : public WIFIImpl
{
private:
    n_io_handle_t usart_handle;
public:
    ESP8266Wifi(n_io_handle_t handle)
        : usart_handle(handle)
    {
    }

    int status()
    {
        n_io_printf(usart_handle, "AT\r\n");
        char buffer[10];

        do
        {
            n_io_readline(usart_handle, buffer, 10);
        } while(strcmp(buffer, "OK\r\n") != 0);
    }

    int restart()
    {
        n_io_printf(usart_handle, "AT+RST\r\n");
        char buffer[10];

        do
        {
            n_io_readline(usart_handle, buffer, 10);
        } while(strcmp(buffer, "OK\r\n") != 0);
    }

    int set_mode(n_wifi_mode_t)
    {
    }

    int connect(const char *, const char *)
    {
    }

    n_io_handle_t open_io(const char *, int)
    {
    }

    int close()
    {
    }

    ~ESP8266Wifi()
    {
    }

private:
    int exec(const char *command, const char *expectedOutput);
};

extern "C"
{
    n_wifi_t n_esp8266_open_wifi(n_io_handle_t usart)
    {
        return new ESP8266Wifi(usart);
    }

}
