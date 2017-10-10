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
        n_io_write(handle, str, len + 1);
        free(str);
    }

    char *n_io_readline(n_io_handle_t handle, char *buffer, size_t size)
    {
        if(!handle)
        {
            return NULL;
        }

        bool internal = false;
        if(buffer == NULL)
        {
            internal = true;
            buffer = (char *) malloc(50);
            size = 50;
        }

        int idx = 0;
        char ch;

        while((ch = n_io_getch(handle)) != '\n')
        {
            buffer[idx++] = ch;

            if(idx == size - 1)
            {
                if(internal)
                {
                    char *rbuffer = (char *) realloc(buffer, size + 10);
                    if(rbuffer == NULL)
                    {
                        break;
                    }
                    buffer = rbuffer;
                    size += 10;
                }
                else
                {
                    break;
                }
            }
        }

        if(idx < size - 1)
        {
            buffer[idx++] = '\n';
        }

        buffer[idx] = '\0';
        return buffer;
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
