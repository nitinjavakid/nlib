#include <delay.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

void n_delay_wait(int seconds, n_delay_sleep_mode_t mode)
{
    int count = seconds/8;

    cli();
    wdt_enable(WDTO_8S);
    sei();

    set_sleep_mode(mode);
    while(count > 0)
    {
        WDTCSR |= (1<<WDIE);
        --count;
        sleep_cpu();
    }
}
