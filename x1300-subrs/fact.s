@ lab1-asm/fact.s
@ Copyright (c) 2018-20 J. M. Spivey

        .syntax unified
        .global func

        .text
        .thumb_func
func:
@@@ Factorials using a loop: keep x in r4, y in r5 and maintain the
@@@ relationship fac(a) = fac(x) * y
        push {r4, r5, lr}
        movs r4, r0
        movs r5, #1

again:
        cmp r4, #0
        beq exit

        @@ Call subroutine to multiply x and y
        movs r0, r4
        movs r1, r5
        bl mult
        movs r5, r0

        subs r4, r4, #1
        b again
exit:   
        movs r0, r5
        pop {r4, r5, pc}


        .thumb_func
mult:   
@@@ Keep x and y in r0 and r1; compute the result z in r2, maintaining
@@@ the relationship a * b = x * y + z

        movs r2, #0
loop:   
        cmp r0, #0
        beq done

        subs r0, r0, #1
        adds r2, r2, r1
        b loop

done:
        movs r0, r2
        bx lr

