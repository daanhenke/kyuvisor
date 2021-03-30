%include "vcpu/CPUContext.asm"

global CPUContext_Capture
CPUContext_Capture:
    ; Push flags to the stack
    pushfq
    
    ; Set all general purpose registers
    mov CPUContext.rax[rcx], rax
    mov CPUContext.rcx[rcx], rcx
    mov CPUContext.rdx[rcx], rdx
    mov CPUContext.rbx[rcx], rbx
    mov CPUContext.rbp[rcx], rbp
    mov CPUContext.rsi[rcx], rsi
    mov CPUContext.rdi[rcx], rdi
    mov CPUContext.r8[rcx], r8
    mov CPUContext.r9[rcx], r9
    mov CPUContext.r10[rcx], r10
    mov CPUContext.r11[rcx], r11
    mov CPUContext.r12[rcx], r12
    mov CPUContext.r13[rcx], r13
    mov CPUContext.r14[rcx], r14
    mov CPUContext.r15[rcx], r15

    ; Get rsp from stack
    lea rax, [rsp + 16]
    mov CPUContext.rsp[rcx], rax

    ; Get return address from the stack
    mov rax, [rsp + 8]
    mov CPUContext.rip[rcx], rax

    ; Get flags from the stack
    mov rax, [rsp]
    mov CPUContext.rflags[rcx], rax
    

    xor rax, rax
    add rsp, 8
    ret