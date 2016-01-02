/**
 * Main file for OS demo
 */

#include "kos.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega48");

static KOS_Semaphore *sem;

static uint8_t val;

static uint8_t st[128];
void the_task(void)
{
    TCCR0B |= (1 << CS00);
    TIMSK0 |= (1 << TOIE0);
    while (1)
    {
        kos_semaphore_pend(sem);
        TCCR0B = 0;

        val++;
    }
}

int main(void)
{
    kos_init();

    sem = kos_semaphore_init(0);

    kos_new_task(&the_task, &st[127]);

    kos_run();

    return 0;
}

ISR(TIMER0_OVF_vect)
{
    kos_isr_enter();
    kos_semaphore_post(sem);
    kos_isr_exit();
}

