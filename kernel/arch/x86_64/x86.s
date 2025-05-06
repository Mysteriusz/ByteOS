.section .text

.global CLFLUSH
CLFLUSH:
    movq %rdi, %rax
    clflush (%rax)
    ret

.global CLFLUSHOPT
CLFLUSHOPT:
    movq %rdi, %rax
    clflushopt (%rax)
    ret

.global LFENCE
LFENCE:
    lfence
    ret
.global SFENCE
SFENCE:
    sfence
    ret
.global MFENCE
MFENCE:
    mfence
    ret
