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

    int n_wifi_connect(n_wifi_t hnd, const char *endpoint, const char *password)
    {
        if(!hnd)
        {
            return -1;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->connect(endpoint, password);
    }

    n_io_handle_t n_wifi_open_io(n_wifi_t hnd, const char *ipaddress, int port)
    {
        if(!hnd)
        {
            return NULL;
        }

        WIFIImpl *impl = static_cast<WIFIImpl *>(hnd);
        return impl->open_io(ipaddress, port);
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
}
