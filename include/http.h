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

#ifndef N_HTTP_H
#define N_HTTP_H

#include <io.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void* n_http_object_t;
    typedef n_http_object_t n_http_request_t;
    typedef n_http_object_t n_http_response_t;

    n_http_request_t  n_http_new_request();
    n_http_response_t n_http_new_response();
    void n_http_destroy(n_http_object_t);

    void n_http_request_read_from_stream(n_http_request_t, n_io_handle_t);
    void n_http_response_read_from_stream(n_http_response_t, n_io_handle_t);

    void n_http_request_write_from_stream(n_http_request_t, n_io_handle_t);
    void n_http_response_write_from_stream(n_http_response_t, n_io_handle_t);

    void n_http_set_header(n_http_object_t, const char *, const char *);
    const char *n_http_get_header(n_http_object_t, const char *);

    void n_http_request_set_method(n_http_request_t, const char *);
    const char *n_http_request_get_method(n_http_request_t);

    void n_http_request_set_uri(n_http_request_t, const char *);
    const char *n_http_request_get_uri(n_http_request_t);

    void n_http_response_set_status(n_http_response_t, unsigned int);
    unsigned int n_http_response_get_status(n_http_response_t);

#ifdef __cplusplus
}
#endif

#endif
