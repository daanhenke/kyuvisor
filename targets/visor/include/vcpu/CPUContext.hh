#pragma once

#include <stdint.h>
#include "config.hh"

namespace kyu
{
    typedef struct _CPUContext
    {
        uint64_t rax;
        uint64_t rcx;
        uint64_t rdx;
        uint64_t rbx;
        uint64_t rsp;
        uint64_t rbp;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t r8;
        uint64_t r9;
        uint64_t r10;
        uint64_t r11;
        uint64_t r12;
        uint64_t r13;
        uint64_t r14;
        uint64_t r15;
        uint64_t rip;
        uint64_t rflags;

        uint64_t Capture() asm("CPUContext_Capture");
        void Restore() asm("CPUContext_Restore");

        void DumpRegisters()
        {
            auto funcs = kyu::config->LoaderFunctions;
            #define PR(reg) funcs.PrintString(#reg ":"); funcs.PrintHex((uint64_t) reg); funcs.PrintString("\n")
            PR(rax);
            PR(rcx);
            PR(rdx);
            PR(rbx);
            PR(rsi);
            PR(rdi);
            PR(rsp);
            PR(rbp);
            PR(rip);
            PR(rflags);

            #undef PR
        }
    } CPUContext;
}