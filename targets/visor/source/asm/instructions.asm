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

global _read_cr0
_read_cr0:
    push rbp
    mov rbp, rsp

    mov rax, cr0

    leave
    ret

global _write_cr0
_write_cr0:
    push rbp
    mov rbp, rsp

    mov cr0, rdi

    leave
    ret

global _read_cr4
_read_cr4:
    push rbp
    mov rbp, rsp

    mov rax, cr4

    leave
    ret

global _write_cr4
_write_cr4:
    push rbp
    mov rbp, rsp

    mov cr4, rdi

    leave
    ret

global _read_msr
_read_msr:
    push rbp
    mov rbp, rsp

    mov rcx, rdi
    rdmsr

    shl rdx, 32
    or rax, rdx

    leave
    ret

global _vmx_on
_vmx_on:
    push rbp
    mov rbp, rsp

    vmxon [rdi]

    leave
    ret


global _inv_vpid
_inv_vpid:
    push rbp
    mov rbp, rsp

    invvpid rdi, [rsi]

    leave
    ret
