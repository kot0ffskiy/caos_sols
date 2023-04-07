.text
.global main
.extern getchar_unlocked
.extern putchar_unlocked
.extern realloc
.extern free
main:
  str x30, [sp, -16]!
  str x21, [sp, -16]!
  str x22, [sp, -16]!
  str x23, [sp, -16]!
  mov x1, 8
  mov x0, 0
  bl realloc
  mov x22, x0
  mov x21, 0
  Loop:
    bl getchar_unlocked
    cmp x0, -1
      beq ZaLoop
    str x0, [x22, x21, lsl 3]
    add x21, x21, 1
    mov x1, x21
    add x1, x1, 1
    mov x23, 8
    mul x1, x1, x23 
    mov x0, x22 
    bl realloc
    mov x22, x0
    b Loop
  ZaLoop:
  LoopZaZaLoop:
    sub x21, x21, 1
    ldr x0, [x22, x21, lsl 3]
    bl putchar_unlocked
    cmp x21, 0
    beq LoopEnd
    b LoopZaZaLoop
  LoopEnd:
  mov x0, x22
  bl free
    ldr  x23, [sp], 16
    ldr  x22, [sp], 16
    ldr  x21, [sp], 16
    ldr  x30, [sp], 16
  ret