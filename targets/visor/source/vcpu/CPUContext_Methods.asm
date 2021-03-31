%include "vcpu/CPUContext.asm"

global CPUContext_Capture
CPUContext_Capture:
    ; Push flags to the stack
    pushfq
    
    ; Set all general purpose registers
    mov [rcx + CPUContext.rax], rax
    mov [rcx + CPUContext.rcx], rcx
    mov [rcx + CPUContext.rdx], rdx
    mov [rcx + CPUContext.rbx], rbx
    mov [rcx + CPUContext.rbp], rbp
    mov [rcx + CPUContext.rsi], rsi
    mov [rcx + CPUContext.rdi], rdi
    mov [rcx + CPUContext.r8], r8
    mov [rcx + CPUContext.r9], r9
    mov [rcx + CPUContext.r10], r10
    mov [rcx + CPUContext.r11], r11
    mov [rcx + CPUContext.r12], r12
    mov [rcx + CPUContext.r13], r13
    mov [rcx + CPUContext.r14], r14
    mov [rcx + CPUContext.r15], r15

    ; Get rsp from stack
    lea rax, [rsp + 16]
    mov [rcx + CPUContext.rsp], rax

    ; Get return address from the stack
    mov rax, [rsp + 8]
    mov [rcx + CPUContext.rip], rax

    ; Get flags from the stack
    mov rax, [rsp]
    mov [rcx + CPUContext.rflags], rax
    

    xor rax, rax
    add rsp, 8
    ret
