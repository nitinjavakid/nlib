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
#include <debug.h>
#include <stdio.h>
#include <delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/wdt.h>

class ESP8266Wifi;

class ESP8266Io : public IOImpl
{
private:
    ESP8266Wifi *module;
    int linkid;
    Buffer *buffer;
    volatile bool closed;
    void (*on_recv_ptr)(int, void *);
    void *on_recv_data;
    char *writebuffer;
    size_t writebuffer_written;
    size_t writebuffer_size;
public:
    ESP8266Io(ESP8266Wifi *mod, int id, size_t buffer_size)
        : module(mod),
          linkid(id),
          closed(false),
          on_recv_ptr(NULL),
          on_recv_data(NULL),
          writebuffer(NULL),
          writebuffer_written(0),
          writebuffer_size(0)
    {
        buffer = new Buffer(buffer_size);
    }

    char   read();
    void   write(char);
    size_t read(void *buffer, size_t size);
    size_t write(const void *buffer, size_t size);
    int    close();
    int    on_recv(void (*)(int, void *), void *);
    void   inject_byte(int byte);
    bool   available()
    {
        return (buffer->available() > 0);
    }

    void   flush();
    void mark_closed()
    {
        closed = true;
    }

    ~ESP8266Io()
    {
        close();
    }
};

class ESP8266Wifi : public WIFIImpl
{
    friend class ESP8266Io;
private:
    n_io_handle_t usart_handle;
    typedef int (* collector_t)(const char *, void *);
    ESP8266Io *connections[5];

    const char *recv_filter = "+IPD,";
    volatile int recv_filter_idx;
    volatile int recv_connection_idx;
    volatile int recv_connection_size;
    volatile char recv_buffer[20];
    volatile char recv_buffer_idx;
    const char *close_filter = ".,CLOSED";
    volatile int close_filter_idx;
    volatile char close_filter_var;
public:
    ESP8266Wifi(n_io_handle_t handle)
        : usart_handle(handle),
          recv_filter_idx(0),
          recv_connection_idx(0),
          recv_connection_size(0),
          recv_buffer_idx(0),
          close_filter_idx(0),
          close_filter_var(0)
    {
        memset(&connections, 0, sizeof(connections));
        memset((void *)recv_buffer, 0, sizeof(recv_buffer));
        auto recv_handler = [](int ch, void *data) -> void {
            ESP8266Wifi *wifi = (ESP8266Wifi *) data;

            if(wifi->recv_buffer_idx)
            {
                if(ch == ':')
                {
                    wifi->recv_buffer[wifi->recv_buffer_idx - 1] = '\0';
                    sscanf((char *)wifi->recv_buffer, "%d,%d", &(wifi->recv_connection_idx), &(wifi->recv_connection_size));

                    wifi->recv_buffer_idx = 0;
                }
                else
                {
                    wifi->recv_buffer[wifi->recv_buffer_idx - 1] = (char) ch;
                    ++(wifi->recv_buffer_idx);
                }

                wifi->close_filter_idx = 0;
                return;
            }

            if(wifi->recv_connection_size > 0)
            {
                if(wifi->connections[wifi->recv_connection_idx] != NULL)
                {
                    wifi->connections[wifi->recv_connection_idx]->inject_byte(ch);
                }
                --(wifi->recv_connection_size);
                wifi->close_filter_idx = 0;
                return;
            }

            // Recieve filter
            if(wifi->recv_filter[wifi->recv_filter_idx] == ch)
            {
                ++(wifi->recv_filter_idx);
            }
            else
            {
                wifi->recv_filter_idx = 0;
            }

            if(wifi->recv_filter[wifi->recv_filter_idx] == '\0')
            {
                wifi->recv_buffer_idx = 1;
                wifi->recv_filter_idx = 0;
            }

            // Closed filter
            if(wifi->close_filter[wifi->close_filter_idx] == '.')
            {
                wifi->close_filter_var = ch;
                ++(wifi->close_filter_idx);
            }
            else if(wifi->close_filter[wifi->close_filter_idx] == ch)
            {
                ++(wifi->close_filter_idx);
            }
            else
            {
                wifi->close_filter_idx = 0;
            }

            if(wifi->close_filter[wifi->close_filter_idx] == '\0')
            {
                if(wifi->connections[wifi->close_filter_var - '0'] != NULL)
                {
                    wifi->connections[wifi->close_filter_var - '0']->mark_closed();
                }
                wifi->close_filter_idx = 0;
            }
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
        if((retval = exec([](const char *line, void *data) -> int {
            if(strncmp(line, "ready", 5) == 0)
            {
                return 1;
            }
            return 0;
        }, NULL, "AT+RST\r\n")) != 0)
        {
            return retval;
        }

        n_delay_loop(5000);

        exec(NULL, NULL, "ATE0\r\n");
    }

    int set_mode(n_wifi_mode_t mode)
    {
        int modeint = 0;
        int retval = 0;
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

        if((retval = exec(NULL, NULL, "AT+CWMODE_CUR=%d\r\n", modeint)) != 0)
        {
            return retval;
        }

        if((retval = exec(NULL, NULL, "AT+CIPMUX=1\r\n")) != 0)
        {
            return retval;
        }

        return retval;
    }

    int get_mode(n_wifi_mode_t *mode);

    int connect(const char *ssid, const char *pwd)
    {
        return exec([](const char *line, void *data) -> int {
            if(strncmp(line, "ready", 5) == 0)
            {
                return 1;
            }
            return 0;
        }, NULL, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", ssid, pwd);
    }

    int set_network(const char *ip, const char *gateway, const char *netmask)
    {
        return exec(NULL, NULL, "AT+CIPSTA_CUR=\"%s\",\"%s\",\"%s\"\r\n", ip, gateway, netmask);
    }

    n_io_handle_t open_io(n_wifi_io_type_t type, const char *ip, int port, size_t buffer_size)
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

        connections[idx] = new ESP8266Io(this, idx, buffer_size);

        return connections[idx];
    }

    int close()
    {
        for(int idx = 0; idx < (sizeof(connections)/sizeof(connections[0])); ++idx)
        {
            if(connections[idx] != NULL)
            {
                n_io_close(connections[idx]);
            }
        }
    }

    int get_ap_nodes(n_wifi_ap_node_t **nodes);

    int free_ap_nodes(n_wifi_ap_node_t *nodes);

    int close_connection(n_io_handle_t handle)
    {
        for(int idx = 0; idx < (sizeof(connections)/sizeof(connections[0])); ++idx)
        {
            if(connections[idx] == handle)
            {
                exec(NULL, NULL, "AT+CIPCLOSE=%d\r\n", idx);
                connections[idx] = NULL;
            }
        }
        return 0;
    }

    ~ESP8266Wifi()
    {
        close();
    }

private:
    int exec(collector_t callback, void *data, const char *fmt, ...);
};

int ESP8266Wifi::exec(collector_t callback, void *data, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    N_VDEBUG(fmt, ap);
    n_io_vsprintf(usart_handle, fmt, ap);
    va_end(ap);

    WDTCSR = WDTCSR & ~(1 << WDIE);
    wdt_enable(WDTO_8S);

    do
    {
        char *buffer = n_io_readline(usart_handle, NULL, 0);

        if(strncmp(buffer, "OK", 2) == 0)
        {
            free(buffer);
            wdt_reset();
            return 0;
        }
        else if(strncmp(buffer, "ERROR", 5) == 0)
        {
            free(buffer);
            wdt_reset();
            return -1;
        }
        else if(strncmp(buffer, "FAIL", 4) == 0)
        {
            free(buffer);
            wdt_reset();
            return -1;
        }
        else
        {
            if(callback)
            {
                int retval = 0;
                if((retval = callback(buffer, data)) != 0)
                {
                    free(buffer);
                    wdt_reset();
                    return retval >> 1;
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
    if(writebuffer == NULL)
    {
        writebuffer = (char *) malloc(10);
        writebuffer_size = 10;
        writebuffer_written = 0;
    }

    if(writebuffer_written == writebuffer_size)
    {
        char *newbuffer = (char *) realloc(writebuffer, writebuffer_size + 10);
        if(newbuffer == NULL)
        {
            flush();
            write(ch);
            return;
        }

        writebuffer = newbuffer;
        writebuffer_size = writebuffer_size + 10;
    }

    writebuffer[writebuffer_written++] = ch;
}

void ESP8266Io::flush()
{
    if(writebuffer && (writebuffer_size > 0))
    {
        if(module->exec(NULL, NULL, "AT+CIPSEND=%d,%d\r\n", linkid, writebuffer_written) == 0)
        {
            if(n_io_write(module->usart_handle, writebuffer, writebuffer_written) == writebuffer_written)
            {
                free(writebuffer);
                writebuffer = NULL;
                writebuffer_size = 0;
                writebuffer_written = 0;

                while(1)
                {
                    char *linebuf = n_io_readline(module->usart_handle, NULL, 0);
                    if(strncmp(linebuf, "SEND OK", 7) == 0)
                    {
                        free(linebuf);
                        return;
                    }
                    else if(strncmp(linebuf, "ERROR", 5) == 0)
                    {
                        free(linebuf);
                        return;
                    }
                    free(linebuf);
                }
            }
            else
            {
                free(writebuffer);
                writebuffer = NULL;
                writebuffer_size = 0;
                writebuffer_written = 0;
            }
        }
        else
        {
            free(writebuffer);
            writebuffer = NULL;
            writebuffer_size = 0;
            writebuffer_written = 0;
        }
    }
}

size_t ESP8266Io::write(const void *buffer, size_t size)
{
    for(int i = 0; i < size; ++i)
    {
        write(((char *) buffer)[i]);
    }
    return size;
}

char ESP8266Io::read()
{
    char retval = -1;
    cli();
    while(!buffer->available())
    {
        if(closed)
        {
            sei();
            return -1;
        }
        power_usart0_enable();
        n_delay_sleep(N_DELAY_IDLE);
        cli();
    }

    if(buffer->available())
    {
        retval = buffer->getch();
    }
    sei();
    return retval;
}

size_t ESP8266Io::read(void *buffer, size_t size)
{
    size_t idx = 0;
    char *chbuffer = (char *) buffer;
    while(idx < size)
    {
        char ch = read();
        if(ch != -1)
        {
            chbuffer[idx++] = ch;
        }
        else
        {
            break;
        }
    }
    return idx;
}

int ESP8266Io::close()
{
    cli();
    module->close_connection(this);
    if(buffer)
    {
        delete buffer;
        buffer = NULL;
    }
    if(writebuffer)
    {
        free(writebuffer);
        writebuffer = NULL;
    }
    sei();
}

void ESP8266Io::inject_byte(int byte)
{
    if(buffer)
    {
        buffer->putch(byte);
    }

    if(on_recv_ptr)
    {
        on_recv_ptr(byte, on_recv_data);
    }
}

int ESP8266Io::on_recv(void (*func)(int, void*), void *data)
{
    on_recv_ptr = func;
    on_recv_data = data;
}

extern "C"
{
    n_wifi_t n_esp8266_open_wifi(n_io_handle_t usart)
    {
        return new ESP8266Wifi(usart);
    }

}
