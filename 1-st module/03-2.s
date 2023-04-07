.text
.global solve

solve:
    sxtw x1, w1
    sxtw x2, w2
    sxtw x3, w3
    mov x4, 0
    mov x5, x0
loop:
    mov x0, x5
    mov x6, x1
    mov x7, x2    
    mul x0, x0, x4
    mul x0, x0, x4
    mul x0, x0, x4
    mul x6, x6, x4
    mul x6, x6, x4
    mul x7, x7, x4
    add x0, x0, x6
    add x0, x0, x7
    add x0, x0, x3
    cmp x0, 0
    beq afterloop
    add x4, x4, 1
    cmp x4, 255
    bne loop
afterloop:
    mov x0, x4
ret