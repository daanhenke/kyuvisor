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

global _store_gdt
_store_gdt:
    push rbp
    mov rbp, rsp

    sgdt [rdi]

    leave
    ret

global _load_gdt
_load_gdt:
    push rbp
    mov rbp, rsp

    lgdt [rdi]

    leave
    ret

global _load_tr
_load_tr:
    push rbp
    mov rbp, rsp

    ltr [rdi]

    leave
    ret

global _vmx_on
_vmx_on:
    push rbp
    mov rbp, rsp

    vmxon [rdi]

    mov rax, 1
    jbe .fail
    jmp .no_error
.fail:
    mov rax, 0
.no_error:

    leave
    ret


global _inv_vpid
_inv_vpid:

    xor rax, rax

    invvpid rdi, [rsi]
    ja no_error

    mov rax, 0x1
no_error:
    ret
