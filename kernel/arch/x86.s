.section .text
.global CLFLUSH
CLFLUSH:
    movq %rdi, %rax
    clflush (%rax)
    mfence
    sfence
    mfence
    ret
