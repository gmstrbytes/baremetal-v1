@@@ mpx-m0.s
@@@ Copyright (c) 2018 J. M. Spivey        

@@@ Hardware multiplexing for the ARM Cortex-M0

        .syntax unified
        .text

@@@ setstack -- enter process mode
        .global setstack
        .thumb_func
setstack:
        msr psp, r0             @ Set up the stack
        movs r0, #2             @ Use psp for stack pointer
        msr control, r0
        isb                     @ Drain the pipeline
        bx lr
        
@@@ Stack layout for interrupt frames (16 words, 64 bytes)
@@@ --------------------------------------
@@@ 15  PSR  Status register
@@@ 14  PC   Program counter
@@@ 13  LR   Link register
@@@ 12  R12
@@@ 11  R3
@@@ 10  R2           (Saved by hardware)
@@@  9  R1
@@@  8  R0
@@@ --------------------------------------
@@@  7  R11   
@@@  6  R10
@@@  5  R9
@@@  4  R8           (Saved manually)
@@@  3  R7
@@@  2  R6
@@@  1  R5
@@@  0  R4  <-- Stack pointer
@@@ --------------------------------------

@@@ isave -- save context for system call
        .macro isave
        mrs r0, psp             @ Get thread stack pointer
        subs r0, #32
        movs r1, r0
        stm r1!, {r4-r7}        @ Save low regs on thread stack
        mov r4, r8              @ Copy from high to low
        mov r5, r9
        mov r6, r10
        mov r7, r11
        stm r1!, {r4-r7}        @ Save high regs in thread stack
        .endm                   @ Return new thread sp

@@@ irestore -- restore context after system call
        .macro irestore         @ Expect process sp in r0
        movs r1, r0
        adds r0, #16
        ldm r0!, {r4-r7}        @ Restore high registers
        mov r8, r4              @ Copy from low to high
        mov r9, r5
        mov r10, r6
        mov r11, r7
        ldm r1!, {r4-r7}        @ Restore low registers
        msr psp, r0             @ Set stack pointer for thread
        .endm

@@@ svc_handler -- handler for SVC interrupt (system call)
        .global svc_handler
        .thumb_func
svc_handler:
        push {lr}               @ Push lr on main stack
        isave                   @ Complete saving of state
        bl system_call          @ Perform system call
        irestore                @ Restore manually saved state
        pop {pc}                @ Return to new thread

@@@ pendsv_handler -- handler for PendSV interupt (context switch)
        .global pendsv_handler
        .thumb_func
pendsv_handler:
        push {lr}
        isave
        bl cxt_switch
        irestore
        pop {pc}

        .pool

@@@ lock -- disable interrupts
        .global lock
        .thumb_func
lock:                           
        mrs r0, primask         @ Save current state for restore
        ldr r1, =prev_mask
        str r0, [r1]
        cpsid i
        bx lr

@@@ unlock -- enable interrupts
        .global unlock
        .thumb_func
unlock:
        cpsie i
        bx lr

@@@ restore -- restore previous interrupt setting (used by kprintf)
        .global restore
        .thumb_func
restore:
        ldr r1, =prev_mask      @ Get previously saved state
        ldr r0, [r1]
        msr primask, r0         @ Restore it
        bx lr
        
        .bss
        .align 2
prev_mask:
        .space 4                @ Previous interrupt setting

