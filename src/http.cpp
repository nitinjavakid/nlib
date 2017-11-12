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

#include <http.h>
#include <io_impl.h>
#include <string.h>
#include <stdio.h>
#include <debug.h>

class HeaderContainer
{
private:
    unsigned int idx;
    char *names[10];
    char *values[10];
public:
    HeaderContainer()
    {
        reset();
    }

    virtual ~HeaderContainer()
    {
        for(int i = 0; i < idx; ++i)
        {
            free(names[i]);
            free(values[i]);
        }
    }

    void reset()
    {
        idx = 0;
    }
    void set_header(const char *name, const char *value);
    const char *get_header(const char *name);
    unsigned int count()
    {
        return idx;
    }

    const char *name(unsigned int i)
    {
        if(i >= idx)
        {
            return NULL;
        }

        return names[i];
    }

    const char *value(unsigned int i)
    {
        if(i >= idx)
        {
            return NULL;
        }

        return values[i];
    }

    virtual void read_from_stream(IOImpl *);
    virtual void write_to_stream(IOImpl *);
};

void HeaderContainer::set_header(const char *name_str, const char *value_str)
{
    if(idx >= (sizeof(names) / sizeof(names[0])))
    {
        return;
    }

    names[idx] = strdup(name_str);
    values[idx] = strdup(value_str);
    ++idx;
}

const char *HeaderContainer::get_header(const char *name_str)
{
    for(int i = 0; i < idx; ++i)
    {
        if(strcasecmp(name(i), name_str) == 0)
        {
            return value(i);
        }
    }

    return NULL;
}

void HeaderContainer::read_from_stream(IOImpl *io)
{
    char *line = NULL;
    while(true)
    {
        line = n_io_readline(io, NULL, 0);
        if(line == NULL)
        {
            break;
        }

        if(strcmp(line, "\r\n") == 0)
        {
            free(line);
            break;
        }

        char *ptr = strchr(line, ':');
        if(ptr == NULL)
        {
            free(line);
            break;
        }

        *ptr = '\0';

        char *value = (ptr + 2);
        ptr = strchr(value, '\r');
        if(ptr == NULL)
        {
            free(line);
            break;
        }

        *ptr = '\0';

        set_header(line, value);

        free(line);
    }
}

void HeaderContainer::write_to_stream(IOImpl *io)
{
    for(int i = 0; i < idx; ++i)
    {
        n_io_printf(io, "%s: %s\r\n", name(i), value(i));
    }

    n_io_printf(io, "\r\n");
}

class HTTPRequest : public HeaderContainer
{
    char method[10];
    char uri[10];
public:
    HTTPRequest()
    {
    }

    void set_uri(const char *uri_str)
    {
        strcpy(uri, uri_str);
    }

    const char *get_uri()
    {
        return uri;
    }

    void set_method(const char *method_str)
    {
        strcpy(method, method_str);
    }

    const char *get_method()
    {
        return method;
    }

    void read_from_stream(IOImpl *);
    void write_to_stream(IOImpl *);
};

void HTTPRequest::read_from_stream(IOImpl *io)
{
    char *line = n_io_readline(io, NULL, 0);
    if(line == NULL)
    {
        return;
    }

    char *ptr = strchr(line, ' ');
    if(ptr == NULL)
    {
        free(line);
        return;
    }

    *ptr = '\0';

    ptr = strchr(ptr + 1, '\r');
    if(ptr == NULL)
    {
        free(line);
        return;
    }

    *ptr = '\0';

    set_method(line);
    set_uri(line + strlen(method) + 1);

    free(line);

    HeaderContainer::read_from_stream(io);
}

void HTTPRequest::write_to_stream(IOImpl *io)
{
    n_io_printf(io, "%s %s HTTP/1.1\r\n", method, uri);

    HeaderContainer::write_to_stream(io);
}

class HTTPResponse : public HeaderContainer
{
    unsigned int status;
public:
    HTTPResponse()
    {
    }

    void set_status(unsigned int status_int)
    {
        status = status_int;
    }

    unsigned int get_status()
    {
        return status;
    }

    void read_from_stream(IOImpl *);
    void write_to_stream(IOImpl *);
};

void HTTPResponse::read_from_stream(IOImpl *io)
{
    char *line = n_io_readline(io, NULL, 0);
    if(line == NULL)
    {
        return;
    }

    char *ptr = strchr(line, ' ');
    if(ptr == NULL)
    {
        free(line);
        return;
    }

    char *status_str = ptr + 1;
    ptr = strchr(ptr + 1, ' ');
    if(ptr == NULL)
    {
        free(line);
        return;
    }

    *ptr = '\0';

    set_status(atoi(status_str));

    free(line);

    HeaderContainer::read_from_stream(io);
}

static const char *code_to_str(unsigned int status)
{
    /*    switch(status)
    {
    case 100:
        return "Continue";
    case 200:
        return "OK";
    case 201:
        return "Created";
    case 202:
        return "Accepted";
    case 203:
        return "Non-Authoritative Information";
    case 204:
        return "No Content";
    case 205:
        return "Reset Content";
    case 206:
        return "Partial Content";
    case 300:
        return "Multiple Choices";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Found";
    case 303:
        return "See Other";
    case 304:
        return "Not Modified";
    case 305:
        return "Use Proxy";
    case 307:
        return "Temporary Redirect";
    case 400:
        return "Bad Request";
    case 401:
        return "Unauthorized";
    case 402:
        return "Payment Required";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 406:
        return "Not Acceptable";
    case 407:
        return "Proxy Authentication Required";
    case 408:
        return "Request Time-out";
    case 409:
        return "Conflict";
    case 410:
        return "Gone";
    case 411:
        return "Length Required";
    case 412:
        return "Precondition Failed";
    case 413:
        return "Request Entity Too Large";
    case 414:
        return "Request-URI Too Large";
    case 415:
        return "Unsupported Media Type";
    case 416:
        return "Requested range not satisfiable";
    case 417:
        return "Expectation Failed";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 502:
        return "Bad Gateway";
    case 503:
        return "Service Unavailable";
    case 504:
        return "Gateway Time-out";
    case 505:
        return "HTTP Version not supported";
        } */
    return NULL;
}

void HTTPResponse::write_to_stream(IOImpl *io)
{
    n_io_printf(io, "HTTP/1.1 %d %s\r\n", status, code_to_str(status));

    HeaderContainer::write_to_stream(io);
}

extern "C"
{
    n_http_request_t  n_http_new_request()
    {
        return new HTTPRequest();
    }

    n_http_response_t n_http_new_response()
    {
        return new HTTPResponse();
    }

    void n_http_destroy(n_http_object_t obj)
    {
        delete static_cast<HeaderContainer *>(obj);
    }

    void n_http_request_read_from_stream(n_http_request_t req, n_io_handle_t io)
    {
        if(req == NULL)
        {
            return;
        }

        HTTPRequest *request = static_cast<HTTPRequest*>(req);
        request->read_from_stream(static_cast<IOImpl *>(io));
    }

    void n_http_response_read_from_stream(n_http_response_t res, n_io_handle_t io)
    {
        if(res == NULL)
        {
            return;
        }

        HTTPResponse *response = static_cast<HTTPResponse*>(res);
        response->read_from_stream(static_cast<IOImpl *>(io));
    }

    void n_http_request_write_to_stream(n_http_request_t req, n_io_handle_t io)
    {
        if(req == NULL)
        {
            return;
        }

        HTTPRequest *request = static_cast<HTTPRequest*>(req);
        request->write_to_stream(static_cast<IOImpl *>(io));
    }

    void n_http_response_write_to_stream(n_http_response_t res, n_io_handle_t io)
    {
        if(res == NULL)
        {
            return;
        }

        HTTPResponse *response = static_cast<HTTPResponse*>(res);
        response->write_to_stream(static_cast<IOImpl *>(io));
    }

    void n_http_set_header(n_http_object_t obj, const char *name, const char *value)
    {
        if(obj == NULL)
        {
            return;
        }

        HeaderContainer *headers = static_cast<HeaderContainer*>(obj);
        headers->set_header(name, value);
    }

    const char *n_http_get_header(n_http_object_t obj, const char *name)
    {
        if(obj == NULL)
        {
            return NULL;
        }

        HeaderContainer *headers = static_cast<HeaderContainer*>(obj);
        return headers->get_header(name);
    }

    void n_http_request_set_method(n_http_request_t req, const char *method)
    {
        if(req == NULL)
        {
            return;
        }

        HTTPRequest *request = static_cast<HTTPRequest*>(req);
        request->set_method(method);
    }

    const char *n_http_request_get_method(n_http_request_t req)
    {
        if(req == NULL)
        {
            return NULL;
        }

        HTTPRequest *request = static_cast<HTTPRequest*>(req);
        return request->get_method();
    }

    void n_http_request_set_uri(n_http_request_t req, const char *uri)
    {
        if(req == NULL)
        {
            return;
        }

        HTTPRequest *request = static_cast<HTTPRequest*>(req);
        request->set_uri(uri);
    }

    const char *n_http_request_get_uri(n_http_request_t req)
    {
        if(req == NULL)
        {
            return NULL;
        }

        HTTPRequest *request = static_cast<HTTPRequest*>(req);
        return request->get_uri();
    }

    void n_http_response_set_status(n_http_response_t res, unsigned int status)
    {
        if(res == NULL)
        {
            return;
        }

        HTTPResponse *response = static_cast<HTTPResponse*>(res);
        response->set_status(status);
    }

    unsigned int n_http_response_get_status(n_http_response_t res)
    {
        if(res == NULL)
        {
            return 0;
        }

        HTTPResponse *response = static_cast<HTTPResponse*>(res);
        return response->get_status();
    }
}
