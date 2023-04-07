.text
.global main
.extern getchar_unlocked
.extern putchar_unlocked
main:
  str x30, [sp, -8]! 
    Loop:
        bl getchar_unlocked
        cmp x0, -1
        beq Zaloop
        cmp x0, 48
        bge check
        b Loop
    check:
        cmp x0, 57
        ble print_
        b Loop
    print_:
        bl putchar_unlocked
        b Loop
    Zaloop:
    ldr  x30,[sp],8
    ret
    