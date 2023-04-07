.intel_syntax noprefix
.text
.global sum

sum:
  push rbx
  mov rax, rcx     // R
  mov rcx, 0
  cmp rdi, 0
  je ZALOOP
LOOP: 
  mov ebx, [rsi + 4 * rcx]
  add ebx, [rdx + 4 * rcx]
  mov [rax + 4 * rcx], ebx
  inc rcx
  cmp rcx, rdi
  jl LOOP
ZALOOP:
pop rbx
ret