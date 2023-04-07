.text
.global sum

sum:
    mov x4, 1
loop:    
    ldr x3, [x2]
    add x2, x2, 4
    add x0, x0, x3
    cmp x4, x1
    bge afterloop
    add x4, x4, 1
    b loop
afterloop:
ret