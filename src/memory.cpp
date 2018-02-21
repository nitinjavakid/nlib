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

#include <stdlib.h>
#ifdef MEMDEBUG
#include <debug.h>
#endif

void * operator new(size_t size)
{
#ifndef MEMDEBUG
    return malloc(size);
#else
    void * ptr = malloc(size);
    N_DEBUG("m=%u", ptr);
    return ptr;
#endif
}

void operator delete(void * ptr)
{
#ifdef MEMDEBUG
    N_DEBUG("f=%u", ptr);
#endif
    free(ptr);
}
