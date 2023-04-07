.intel_syntax noprefix
.text
.global avx_sin

avx_sin:                                                   
  movsd xmm1, xmm0          //x 
  movsd xmm7, xmm0    
  mov rdx, -1
  cvtsi2sd xmm6, rdx        // znak
  movsd xmm4, xmm6          // -1 (change znak)
  mov rax, 3
  cvtsi2sd xmm8, rax
  mov rbx, 2
  cvtsi2sd xmm2, rbx
  mov rcx, 1
  cvtsi2sd xmm9, rcx
  mulsd xmm2, xmm8        // fact(3)
  mulsd xmm7, xmm1        // x * x

  Metka:
    movsd xmm10, xmm0       

    mulsd xmm1, xmm7        // x * k
    movsd xmm5, xmm1        //remember x
    divsd xmm5, xmm2        // x / i!
    mulsd xmm5, xmm6        // znak * (x / i!)
    mulsd xmm6, xmm4        // change znak
    addsd xmm0, xmm5        // res += znak * (x / i!)
    
    addsd xmm8, xmm9
    mulsd xmm2, xmm8
    addsd xmm8, xmm9
    mulsd xmm2, xmm8  
  
    comisd xmm0, xmm10
    jne Metka 
  ret