#include <io.h>
#include <io_impl.h>
#include <stdarg.h>
#include <stdio.h>

extern "C"
{
    char n_io_getch(n_io_handle_t handle)
    {
        if(!handle)
        {
            return -1;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        return impl->read();
    }

    void n_io_putch(n_io_handle_t handle, char val)
    {
        if(!handle)
        {
            return;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        impl->write(val);
    }

    int n_io_read(n_io_handle_t handle, void *buffer, size_t size)
    {
        if(!handle)
        {
            return -1;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        return impl->read(buffer, size);
    }

    int n_io_write(n_io_handle_t handle, const void *buffer, size_t size)
    {
        if(!handle)
        {
            return -1;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        return impl->write(buffer, size);
    }

    void n_io_printf(n_io_handle_t handle, const char *fmt, ...)
    {
        char *str = NULL;
        int len = 0;
        int idx = 0;
        va_list ap;
        va_start(ap, fmt);
        len = vsnprintf(NULL, 0, fmt, ap);
        str = (char *) calloc(len + 1, 1);
        len = vsnprintf(str, len + 1, fmt, ap);
        va_end(ap);
        n_io_write(handle, str, len);
        free(str);
    }

    int n_io_close(n_io_handle_t handle)
    {
        if(!handle)
        {
            return -1;
        }

        IOImpl *impl = static_cast<IOImpl *>(handle);
        int retval = impl->close();
        delete impl;
        return retval;
    }
}
