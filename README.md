# Kevin's RTOS (KOS)
## Kevin Cuzner


This is a preemptive task-based real-time operating system designed to consume
as few resources as possible while increasing the effective utilization of
systems using AVR microcontrollers.

The motivation for this project stems from my own desire to write something like
this. There are many other options available which are likely better written and
easier to use, but I'm going to have more fun doing it this way.

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

## Scheduling & Dispatching


