.section .text

.global __aeabi_uidivmod
__aeabi_uidivmod:
    push {lr}
    udiv r2, r0, r1
    mul  r3, r2, r1
    sub  r3, r0, r3
    mov  r0, r2
    mov  r1, r3
    pop {pc}

.global __aeabi_uldivmod
__aeabi_uldivmod:
    push {r4, r5, lr}
    mov  r4, r0
    mov  r5, r1
    udiv r0, r4, r5
    mul  r1, r0, r5
    sub  r1, r4, r1
    pop  {r4, r5, pc}

.global __aeabi_uidiv
__aeabi_uidiv:
    udiv r0, r0, r1
    bx lr
