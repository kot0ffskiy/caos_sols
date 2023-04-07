.intel_syntax noprefix
.text
.global dot_product

dot_product:
  xorpd xmm0, xmm0     
  mov rax, 0
  cmp rdi, 8
  jl LastMetka                     //  set bit-bound
  sub rdi, 8
  vxorpd ymm4, ymm4, ymm4          //  res 

  Metka:
    vmovups ymm1, [rsi + rax * 4]  //  first vec  
    vmovups ymm2, [rdx + rax * 4]  //  sec vec
    vdpps ymm1, ymm1, ymm2, 0xFF   //  scal 
    vaddps ymm4, ymm4, ymm1        //  add scal to result
    add rax, 8
    cmp rax, rdi
    jl Metka
    vextractf128 xmm0, ymm4, 1
    addss xmm4, xmm0
    movss xmm0, xmm4
    add rdi, 8                     //  set input value (line 7)
  LastMetka:
    cmp rax, rdi
    je End
    movss xmm1, [rsi + rax * 4]
    movss xmm2, [rdx + rax * 4]
    mulss xmm2, xmm1
    addss xmm0, xmm2
    add rax, 1
    jmp LastMetka
  End:
  ret
