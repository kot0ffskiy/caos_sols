#include <sys/syscall.h>

.intel_syntax noprefix
.text
.global _start

_start:
    mov rax, __NR_read
    mov rdi, 0
    mov rsi, input_ptr
    mov rdx, 1
    syscall

Loop:

    cmp rax, 0
    je End
    mov rax, __NR_write
    mov rdi, 1
    mov rsi, input_ptr
    mov rdx, 1
    syscall

    mov rax, __NR_read
    mov rdi, 0
    mov rsi, input_ptr
    mov rdx, 1
    syscall

    jmp Loop

End:
    mov rax, __NR_exit
    mov rbx, 0
    syscall

.data
input: .byte 0
input_ptr: .quad input