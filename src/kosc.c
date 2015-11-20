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
 * SREG
 */

static KOS_Task tasks[KOS_MAX_TASKS + 1];
static uint8_t next_task = 0;
static KOS_Task *task_head;
KOS_Task *kos_current_task;

static uint8_t kos_idle_task_stack[KOS_IDLE_TASK_STACK];
static void kos_idle_task(void)
{
    while (1) { }
}

void kos_init(void)
{
    kos_new_task(&kos_idle_task, &kos_idle_task_stack[KOS_IDLE_TASK_STACK - 1]);
}

void kos_new_task(KOS_TaskFn task, void *sp)
{
    int8_t i;
    uint8_t *stack = sp;
    KOS_Task *tcb;

    //make space for pc, sreg, and 32 register
    stack[0] = (uint16_t)task & 0xFF;
    stack[-1] = (uint16_t)task >> 8;
    for (i = -2; i > -34; i--)
    {
        stack[i] = 0;
    }
    stack[-34] = 0x80; //sreg, interrupts enabled
    
    //create the task structure
    tcb = &tasks[next_task++];
    tcb->sp = stack - 35;
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
    KOS_Task *task = task_head;
    while (task->status != TASK_READY)
        task = task->next;

    if (task != kos_current_task)
    {
        sei();
        kos_dispatch(task);
    }
}

