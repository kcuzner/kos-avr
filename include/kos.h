/**
 * Kevin's RTOS
 */
#ifndef _KOS_H_
#define _KOS_H_

#include <stdint.h>

#include "kos_settings.h"

typedef enum { TASK_READY, TASK_SEMAPHORE } KOS_TaskStatus;

typedef struct KOS_Task {
    void *sp;
    KOS_TaskStatus status;
    struct KOS_Task *next;
    void *status_pointer;
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
 * Enters the mutex
 */
void kos_mutex_enter(void);

/**
 * Exits the mutex
 */
void kos_mutex_exit(void);

#ifdef KOS_SEMAPHORE

typedef struct {
    int8_t value;
} KOS_Semaphore;

/**
 * Initializes a new semaphore
 */
KOS_Semaphore *kos_semaphore_init(int8_t value);

/**
 * Posts to a semaphore
 */
void kos_semaphore_post(KOS_Semaphore *sem);

/**
 * Pends from a semaphore
 */
void kos_semaphore_pend(KOS_Semaphore *sem);

#endif //KOS_SEMAPHORE

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
