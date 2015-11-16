/**
 * Kevin's RTOS
 */
#ifndef _KOS_H_
#define _KOS_H_

#include <stdint.h>

typedef enum { TASK_READY, TASK_WAITING_ISR } KOS_TaskStatus;

typedef struct KOS_Task {
    void *sp;
    KOS_TaskStatus status;
    struct KOS_Task *next;
} KOS_Task;

typedef void (*KOS_TaskFn)(void);

extern KOS_Task *kos_current_task;

/**
 * Initializes the KOS kernel
 */
void kos_init(void);

/**
 * Creates a new task
 */
void kos_new_task(KOS_TaskFn task, void *sp);

/**
 * Runs the kernel
 */
void kos_run(void);

/**
 * Runs the scheduler
 */
void kos_schedule(void);

/**
 * Dispatches the passed task, saving the context of the current task
 */
void kos_dispatch(KOS_Task *next);

#endif //_KOS_H_
