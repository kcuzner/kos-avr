/**
 * Kevin's RTOS
 */
#ifndef _KOS_H_
#define _KOS_H_

#include <stdint.h>

#include "kos_settings.h"

typedef enum { TASK_READY, TASK_SEMAPHORE, TASK_QUEUE } KOS_TaskStatus;

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
 * Puts KOS in ISR mode
 */
void kos_isr_enter(void);

/**
 * Leaves ISR mode, possibly executing the dispatcher
 */
void kos_isr_exit(void);

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

#ifdef KOS_QUEUE

typedef struct {
    void **messages;
    uint8_t pendIndex;
    uint8_t postIndex;
    uint8_t size;
} KOS_Queue;

/**
 * Initializes a new queue
 */
KOS_Queue *kos_queue_init(void **messages, uint8_t size);

/**
 * Posts to a queue
 */
void kos_queue_post(KOS_Queue *queue, void *message);

/**
 * Pends from a queue
 */
void *kos_queue_pend(KOS_Queue *queue);

#endif //KOS_QUEUE

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
