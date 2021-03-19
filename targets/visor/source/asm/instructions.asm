global _cpuid
_cpuid:
    push rbp
    mov rbp, rsp
    
    push rbx

    mov eax, [rdi]
    mov ecx, [rdi + 8]

    cpuid

    mov [rdi], eax
    mov [rdi + 4], ebx
    mov [rdi + 8], ecx
    mov [rdi + 12], edx

    pop rbx

    leave
    ret