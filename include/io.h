#ifndef N_IO_H
#define N_IO_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void* n_io_handle_t;

    char  n_io_getch(n_io_handle_t);
    void  n_io_putch(n_io_handle_t, char byte);

    int   n_io_read(n_io_handle_t, void *buffer, size_t size);
    int   n_io_write(n_io_handle_t, const void *buffer, size_t size);
    void  n_io_printf(n_io_handle_t, const char *fmt, ...);
    char *n_io_readline(n_io_handle_t, char *buffer, size_t size);
    int   n_io_close(n_io_handle_t);

#ifdef __cplusplus
}
#endif

#endif
