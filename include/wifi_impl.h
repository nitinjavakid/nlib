#ifndef N_WIFI_IMPL_H
#define N_WIFI_IMPL_H

class WIFIImpl
{
public:
    virtual int status() = 0;
    virtual int restart() = 0;
    virtual int set_mode(n_wifi_mode_t) = 0;
    virtual int connect(const char *, const char *) = 0;
    virtual n_io_handle_t open_io(const char *, int) = 0;
    virtual int close() = 0;
    virtual ~WIFIImpl()
    {
    }
};

#endif
