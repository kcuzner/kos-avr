
#include <avr/io.h>
#include <avr/interrupt.h>

    .global kos_dispatch

kos_dispatch:
    // the call to this function should push the return address into the stack.
    // we will now construct saving context. The entire context needs to be
    // saved because it is very possible that this could be called from within
    // an isr that doesn't use the call-used registers and therefore doesn't
    // save them.
    push r31
    push r30
    push r29
    push r28
    push r27
    push r26
    push r25
    push r24
    push r23
    push r22
    push r23
    push r21
    push r20
    push r19
    push r18
    push r17
    push r16
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push r7
    push r6
    push r5
    push r4
    push r3
    push r2
    push r1
    push r0
    in   r0, _SFR_IO_ADDR(SREG)
    push r0
    lds  r26, kos_current_task //dereference current task
    lds  r27, kos_current_task+1
    tst  r26 //test for 16-bit null
    brne kos_dispatch_tsth
    rjmp kos_dispatch_next
kos_dispatch_tsth:
    tst  r27
    breq kos_dispatch_next
    in   r0, _SFR_IO_ADDR(SPL) //load stack pointer
    st   X+, r0 //store it as the first thing in the Task *
    in   r0, _SFR_IO_ADDR(SPH)
    st   X+, r0
kos_dispatch_next:
    mov  r26, r24 //argument to this function, the next Task *, dereferenced
    mov  r27, r23
    sts  kos_current_task, r26 //set current task
    sts  kos_current_task+1, r27
    ld   r0, X+ //load stack pointer
    out  _SFR_IO_ADDR(SPL), r0
    ld   r0, X+
    out  _SFR_IO_ADDR(SPH), r0
    pop  r0
    out  _SFR_IO_ADDR(SREG), r0
    pop  r0
    pop  r1
    pop  r2
    pop  r3
    pop  r4
    pop  r5
    pop  r6
    pop  r7
    pop  r8
    pop  r9
    pop  r10
    pop  r11
    pop  r12
    pop  r13
    pop  r14
    pop  r15
    pop  r16
    pop  r17
    pop  r18
    pop  r19
    pop  r20
    pop  r21
    pop  r22
    pop  r23
    pop  r24
    pop  r25
    pop  r26
    pop  r27
    pop  r28
    pop  r29
    pop  r30
    pop  r31
    reti

