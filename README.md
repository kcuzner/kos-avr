# Kevin's RTOS (KOS)
## Kevin Cuzner


This is a preemptive task-based real-time operating system designed to consume
as few resources as possible while increasing the effective utilization of
systems using AVR microcontrollers.

The motivation for this project stems from my own desire to write something like
this. There are many other options available which are likely better written and
easier to use, but I'm going to have more fun doing it this way.

# Goals

1. Small codebase that is easy enough to read.
2. Small memory footprint.

# Operation

## Tasks

The basic operational unit is a Task. A Task is created from a pointer to the
function to invoke to begin the task and a pointer to the top of the memory to
use as a stack. The absolute minimum stack size is 35 bytes, but this will
likely run into problems since most programs use part of the stack as storage
for variables and such. Tasks should each contain a loop which never exits. The
operating system is optimized for the case where tasks always exist.

The order in which tasks are created sets their priority. The last task created
will have the highest priority.

## Preemption

As a preempting operating system, tasks can be suspended at any time. At the
point where a task is suspended, the next program counter, status register,
and 32 registers are pushed onto the current stack.

In general, preemption will occur in response to an interrupt. This means that
if interrupts are disabled, the task won't be interrupted. Since no two tasks
can have the same priority, there is no time-slicing or round-robin support.
A task will run until it blocks or is interrupted.

## Scheduling & Dispatching

The scheduler is first run with a call to `kos_run()`. When it is invoked, it
will follow the list of tasks in priority order and find the highest priority
task that is ready. It will then invoke the dispatcher on this task.

The dispatcher works by storing the context of the current task on the stack,
overwriting the stack pointer with the next task's stack pointer, popping all
the previously stored register values, and then executing a `ret` or `reti`
(if interrupts are enabled) to jump back into executing the (next, now current)
task.

## Blocking

Tasks are blocked by calling functions which change the state of the current
task to "blocked". In general, they also store a pointer to some data which can
be used to determine how to unblock the task. Examples of blocking functions
are:
- `kos_semaphore_pend`
- `kos_queue_pend`

When these functions are executed, there is a chance that the current task will
have its state changed to blocked. The scheduler will be called and the first
ready task (which won't be the current task, since it was just blocked) will be
dispatched.

## Un-blocking

A task is unblocked simply by changing its status back to "ready". Examples of
functions which do this are:
- `kos_semaphore_post`
- `kos_queue_post`

These functions all traverse the task list, mark a single task as ready (if
needed), and then call the scheduler. If the current task is higher priority
than the task that was unblocked, then no context switch occurs. However, if a
higher priority task is unblocked, then the current task is suspended and the
higher priority task is resumed.

# Conveniences

Any RTOS is almost useless if it just consists of tasks and a dispatcher. It
needs something to allow the tasks to switch. KOS provides the following:
- Semaphores
- Queues (NOTE: There is no full checking, these are implemented as circular
  buffers and will overwrite un-dequeued data).

