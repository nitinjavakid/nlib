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
#include <io_impl.h>
#include <string.h>
#include <buffer.h>

class ESP8266Wifi;

class ESP8266Io : public IOImpl
{
private:
    ESP8266Wifi *module;
    int linkid;
public:
    ESP8266Io(ESP8266Wifi *mod, int id)
        : module(mod), linkid(id)
    {
    }

    char   read();
    void   write(char);
    size_t read(void *buffer, size_t size);
    size_t write(const void *buffer, size_t size);
    int    close();
    int    on_recv(void (*)(int, void *), void *);
};

class ESP8266Wifi : public WIFIImpl
{
    friend class ESP8266Io;
private:
    n_io_handle_t usart_handle;
    typedef int (* collector_t)(const char *, void *);
public:
    ESP8266Wifi(n_io_handle_t handle)
        : usart_handle(handle)
    {
        auto recv_handler = [](int ch, void *data) -> void {
            
        };
        n_io_on_recv(usart_handle, recv_handler, this);
    }

    int status()
    {
        return exec(NULL, NULL, "AT\r\n");
    }

    int restart()
    {
        int retval = 0;
        if((retval = exec(NULL, NULL, "AT+RST\r\n")) != 0)
        {
            return retval;
        }

        if((retval = exec(NULL, NULL, "AT+CIPMUX=1\r\n")) != 0)
        {
            return retval;
        }
        exec(NULL, NULL, "AT\r\n");
    }

    int set_mode(n_wifi_mode_t mode)
    {
        int modeint = 0;
        switch(mode)
        {
        case N_WIFI_MODE_STA:
            modeint = 1;
            break;
        case N_WIFI_MODE_AP:
            modeint = 2;
            break;
        case N_WIFI_MODE_BOTH:
            modeint = 3;
            break;
        }
        return exec(NULL, NULL, "AT+CWMODE=%d\r\n", modeint);
    }

    int get_mode(n_wifi_mode_t *mode);

    int connect(const char *ssid, const char *pwd)
    {
        return exec(NULL, NULL, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);
    }

    int set_network(const char *ip, const char *gateway, const char *netmask)
    {
        return exec(NULL, NULL, "AT+CIPSTA_CUR=\"%s\",\"%s\",\"%s\"\r\n", ip, gateway, netmask);
    }

    n_io_handle_t open_io(n_wifi_io_type_t type, const char *ip, int port)
    {
        int idx = -1;
        const char *typestr = NULL;
        switch(type)
        {
        case N_WIFI_IO_TYPE_TCP:
            typestr = "TCP";
            break;
        case N_WIFI_IO_TYPE_UDP:
            typestr = "UDP";
            break;
        case N_WIFI_IO_TYPE_SSL:
            typestr = "SSL";
            break;
        }
        for(int i = 0; i < 5; ++i)
        {
            if(exec(NULL, NULL, "AT+CIPSTART=%d,\"%s\",\"%s\",%d\r\n", i, typestr, ip, port) == 0)
            {
                idx = i;
                break;
            }
        }

        if(idx == -1)
        {
            return NULL;
        }

        return new ESP8266Io(this, idx);
    }

    int close()
    {
    }

    int get_ap_nodes(n_wifi_ap_node_t **nodes);

    int free_ap_nodes(n_wifi_ap_node_t *nodes);

    ~ESP8266Wifi()
    {
    }

private:
    int exec(collector_t callback, void *data, const char *fmt, ...);
};

int ESP8266Wifi::exec(collector_t callback, void *data, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    n_io_vsprintf(usart_handle, fmt, ap);
    va_end(ap);

    do
    {
        char *buffer = n_io_readline(usart_handle, NULL, 0);

        if(strncmp(buffer, "OK", 2) == 0)
        {
            free(buffer);
            return 0;
        }
        else if(strncmp(buffer, "ERROR", 5) == 0)
        {
            free(buffer);
            return -1;
        }
        else if(strncmp(buffer, "FAIL", 4) == 0)
        {
            free(buffer);
            return -1;
        }
        else
        {
            if(callback)
            {
                if(callback(buffer, data))
                {
                    free(buffer);
                    return 1;
                }
            }
            free(buffer);
        }
    } while(1);

    return 0;
}

int ESP8266Wifi::get_mode(n_wifi_mode_t *mode)
{
    auto collector = [](const char *line, void *data) -> int
    {
        n_wifi_mode_t *ptr = (n_wifi_mode_t *) data;

        if(strncmp(line, "+CWMODE:", 8) == 0)
        {
            if(ptr)
            {
                switch(line[8])
                {
                case '1':
                    *ptr = N_WIFI_MODE_STA;
                    break;
                case '2':
                    *ptr = N_WIFI_MODE_AP;
                    break;
                case '3':
                    *ptr = N_WIFI_MODE_BOTH;
                    break;
                }
            }
        }

        return 0;
    };

    return exec(collector, mode, "AT+CWMODE?\r\n");
}

int ESP8266Wifi::get_ap_nodes(n_wifi_ap_node_t **nodes)
{
    auto collector = [](const char *line, void *data) -> int
    {
        n_wifi_ap_node_t **nodes = (n_wifi_ap_node_t **) data;
        if(nodes == NULL)
        {
            return 0;
        }

        if(strncmp(line, "+CWLAP:", 7) == 0)
        {
            const char *startptr = NULL;
            const char *endptr = NULL;
            n_wifi_ap_node_t *newnode = (n_wifi_ap_node_t *) malloc(sizeof(n_wifi_ap_node_t));

            if(newnode == NULL)
                return 1;

            newnode->ecn = line[8] - '0';
            startptr = &line[11];
            endptr = strchr(startptr, '"');
            newnode->ssid = (char *) malloc(sizeof(char) * ((endptr - startptr) + 1));
            if(newnode->ssid == NULL)
            {
                free(newnode);
                return 1;
            }

            strncpy(newnode->ssid, startptr, endptr - startptr);
            newnode->ssid[endptr - startptr] = '\0';
            newnode->rssi = atoi(endptr + 2);
            startptr = strchr(endptr+1, '"') + 1;
            endptr = strchr(startptr, '"');
            newnode->mac = (char *) malloc(sizeof(char) * ((endptr - startptr) + 1));
            if(newnode->mac == NULL)
            {
                free(newnode->ssid);
                free(newnode);
                return 1;
            }

            strncpy(newnode->mac, startptr, endptr - startptr);
            newnode->mac[endptr - startptr] = '\0';
            newnode->channel = atoi(endptr + 2);
        }
        return 0;
    };

    return exec(collector, nodes, "AT+CWLAP\r\n");
}

int ESP8266Wifi::free_ap_nodes(n_wifi_ap_node_t *nodes)
{
    while(nodes)
    {
        n_wifi_ap_node_t *current = nodes;
        nodes = current->next;
        free(current->ssid);
        free(current->mac);
        free(current);
    }
}

void ESP8266Io::write(char ch)
{
    write(&ch, 1);
}

size_t ESP8266Io::write(const void *buffer, size_t size)
{
    if(module->exec(NULL, NULL, "AT+CIPSEND=%d,%d\r\n", linkid, size) == 0)
    {
        if(n_io_write(module->usart_handle, buffer, size) == size)
        {
            while(1)
            {
                char buffer[50];
                n_io_readline(module->usart_handle, buffer, sizeof(buffer));
                if(strncmp(buffer, "SEND OK", 7) == 0)
                {
                    return size;
                }
                else if(strncmp(buffer, "ERROR", 5) == 0)
                {
                    return -1;
                }
            }
        }
    }

    return -1;
}

char ESP8266Io::read()
{
    return 'c';
}

size_t ESP8266Io::read(void *buffer, size_t size)
{
    return size;
}

int ESP8266Io::close()
{
}

int ESP8266Io::on_recv(void (*)(int, void*), void*)
{
}

extern "C"
{
    n_wifi_t n_esp8266_open_wifi(n_io_handle_t usart)
    {
        return new ESP8266Wifi(usart);
    }

}