#include <sys/syscall.h>

.intel_syntax noprefix
.text
.global _start

_start:
    push r12
    push r13
    push r14
    push r15
    xor r12, r12
    xor r13, r13
    xor r14, r14
    xor r15, r15

    
    mov rbx, 0
    mov rax, __NR_brk
    mov rdi, rbx
    syscall

    mov r14, rax
    mov r15, r14

Loop:
    cmp rbx, 0
    jg Return
    mov rbx, 4000
    add r15, rbx
    mov rdi, r15
    mov rax, __NR_brk
    syscall
    sub r15, rbx
Return:
    mov rax, __NR_read
    mov rdi, 0
    mov rsi, r15
    mov rdx, rbx
    syscall
    cmp rax, 0
    je FindEndString
    sub rbx, rax
    add r15, rax
    jmp Loop

FindEndString:
    cmp r15, r14
    je END
    dec r15
    mov r12, 10
    mov rbx, [r15]
    cmp rbx, r12
    je Output
    inc r15
    mov [r15], r12
Output: 
    mov r13, r15
Loopiter:
    cmp r15, r14
    je CheckForLast
    sub r15, 1
    movb al, [r15]
    cmp al, 10
    je OutLoop
    jmp Loopiter

OutLoop:
    mov rax, __NR_write
    mov rdi, 1
    mov rsi, r15
    add rsi, 1
    mov rdx, r13
    sub rdx, r15
    syscall
    jmp Output

CheckForLast:
    mov rax, __NR_write
    mov rdi, 1
    mov rsi, r15
    mov rdx, r13
    sub rdx, r15
    inc rdx
    syscall
    jmp End

End:
    pop r15
    pop r14
    pop r13
    pop r12
END:
    mov rax, __NR_exit
    mov rdi, 0
    syscall