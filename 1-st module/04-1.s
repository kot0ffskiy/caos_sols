.data
output_string: .asciz "%d\n"
input_string: .string "%d %d"
x1: .word 0
x2: .word 0

  .text

  .global main
  .extern printf
  .extern scanf
main:
  str x30, [sp, -8]!
  ldr x0,  =input_string
  ldr x1, =x1
  ldr x2, =x2
  bl scanf
  ldr x1, =x1
  ldr x2, =x2
  ldr x1, [x1]
  ldr x2, [x2]

  add x1, x2, x1
  ldr x0, =output_string

  bl printf

  ldr x30, [sp], 8
  ret
 