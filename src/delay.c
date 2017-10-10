#include <delay.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

volatile static int count = 0;
ISR(WDT_vect)
{
    --count;
    wdt_disable();
}

void n_delay_wait(int seconds, n_delay_sleep_mode_t mode)
{
    count = seconds/8;

    while(count > 0)
    {
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

void n_delay_sleep(n_delay_sleep_mode_t mode)
{
    set_sleep_mode(mode);
    cli();
    sleep_enable();
    sleep_bod_disable();
    sei();
    sleep_cpu();
    sleep_disable();
    sei();
}
