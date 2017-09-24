#include <delay.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

ISR(WDT_vect)
{
    wdt_disable();
}

void n_delay_wait(int seconds, n_delay_sleep_mode_t mode)
{
    int count = seconds/8;

    while(count > 0)
    {
        --count;
        wdt_enable(WDTO_8S);
        WDTCSR |= (1 << WDIE);
        set_sleep_mode(mode);
        cli();
        sleep_enable();
        sleep_bod_disable();
        sei();
        sleep_cpu();
        sleep_disable();
        sei();
    }
}
