#ifndef N_DELAY_H
#define N_DELAY_H

#include <config.h>

#define n_delay_init() do { SMCR |= 0x01; } while(0)

#ifdef __cplusplus
extern "C"
{
#endif

    void n_delay_wait(int seconds, n_delay_sleep_mode_t mode);
    void n_delay_sleep(n_delay_sleep_mode_t mode);

#ifdef __cplusplus
}
#endif

#endif
