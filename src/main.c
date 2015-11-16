/**
 * Main file for OS demo
 */

#include "kos.h"

#include "avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega48");

int main(void)
{
    kos_init();

    kos_run();

    return 0;
}

