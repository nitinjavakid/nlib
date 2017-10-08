#ifndef N_WIFI_H
#define N_WIFI_H

#include "io.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void* n_wifi_t;

    typedef enum _n_wifi_mode
    {
        N_WIFI_MODE_STA,
        N_WIFI_MODE_AP,
        N_WIFI_MODE_BOTH
    } n_wifi_mode_t;

    typedef enum _n_wifi_channel_type
    {
        N_WIFI_CHANNEL_TYPE_TCP,
        N_WIFI_CHANNEL_TYPE_UDP
    } n_wifi_channel_type_t;

    int n_wifi_status(n_wifi_t);
    int n_wifi_restart(n_wifi_t);
    int n_wifi_set_mode(n_wifi_t, n_wifi_mode_t);
    int n_wifi_connect(n_wifi_t, const char *endpoint, const char *password);

    n_io_handle_t n_wifi_new_io(n_wifi_t, const char *ipaddress, int port);

#ifdef __cplusplus
}
#endif

#endif
