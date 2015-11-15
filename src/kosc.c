/**
 * Main code for KOS
 */

#include <avr/interrupt.h>

#include "kos.h"
#include "kos_settings.h"

/**
 * Context frame format:
 *
 * PC_LOW
 * PC_HIGH
 * r0-31
 *
 * Why no status register? Because a dispatch always occurs as a result of a c
 * function call. The only time we need to save a status register is during an
 * interrupt. Since the scheduler will always push a new context onto the stack
 * in addition to the one created by the ISR (inefficient, but keeps things
 * simple and allows normal avr ISR macros to work)
 */

static Task tasks[MAX_TASKS];
static uint8_t next_task = 0;
static Task *task_head;
Task *kos_current_task;

static uint8_t kos_idle_task_stack[48];
static void kos_idle_task(void)
{
    while (1) { }
}

void kos_init(void)
{
    kos_new_task(&kos_idle_task, &kos_idle_task_stack);
}

void kos_new_task(KOS_TaskFn task, void *sp)
{
    uint8_t i;
    uint8_t *stack;
    Task *tcb;

    //make space for pc, sreg, and 32 register
    stack = (uint8_t*)sp - 35;
    stack[0] = (uint16_t)task & 0xFF;
    stack[1] = (uint16_t)task >> 8;
    for (i = 2; i < 33; i++)
    {
        stack[i] = 0;
    }
    stack[34] = 0x80; //sreg, interrupts enabled
    
    //create the task structure
    tcb = &tasks[next_task++];
    tcb->sp = sp;
    tcb->status = TASK_READY;

    //insert into the task list as the new highest priority task
    if (task_head)
    {
        tcb->next = task_head;
        task_head = tcb;
    }
    else
    {
        task_head = tcb;
    }
}

void kos_run(void)
{
    kos_schedule();
}

void kos_schedule(void)
{
    Task *task = task_head;
    while (task->status != TASK_READY)
        task = task->next;

    if (task != kos_current_task)
    {
        sei();
        kos_dispatch(task);
    }
}

static void Default_IRQHandler(void)
{
}


void ADC_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void ANALOG_COMP_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void EE_READY_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void INT0_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void INT1_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void PCINT0_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void PCINT1_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void PCINT2_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void SPI_STC_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void SPM_READ_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER0_COMPA_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER0_COMPB_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER0_OVF_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER1_CAPT_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER1_COMPA_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER1_COMPB_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER1_OVF_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER2_COMPA_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER2_COMPB_IRQHAndler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TIMER2_OVF_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void TWI_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void USART_RX_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void USART_TX_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void USART_UDRE_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));
void WDT_IRQHandler(void) __attribute__((weak,alias("Default_IRQHandler")));

