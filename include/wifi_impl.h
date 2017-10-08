#ifndef N_WIFI_IMPL_H
#define N_WIFI_IMPL_H

class WIFIImpl
{
public:
    virtual int n_wifi_status(n_wifi_t) = 0;
    virtual int n_wifi_restart(n_wifi_t) = 0;
    virtual int n_wifi_set_mode(n_wifi_t, n_wifi_mode_t) = 0;
    virtual int n_wifi_connect(n_wifi_t, const char *, const char *) = 0;
    virtual n_io_handle_t n_wifi_open_io(n_wifi_t, const char *, int) = 0;
};

#endif
