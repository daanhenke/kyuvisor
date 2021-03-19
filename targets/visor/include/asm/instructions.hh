#pragma once

#include <stdint.h>

#include "macros.hh"

namespace kyu::asm64
{
    typedef union cpuid_regs_t
    {
        struct
        {
            uint32_t eax;
            uint32_t ebx;
            union
            {
                uint32_t ecx;
                struct
                {
                    uint32_t sse3: 1;
                    uint32_t pclmulqdq: 1;
                    uint32_t dtes64: 1;
                    uint32_t monitor: 1;
                    uint32_t ds_cpl: 1;
                    uint32_t vmx: 1;
                };
            };
            uint32_t edx;
        };

        char as_string[12];
        
    } cpuid_regs_t;

    NASM_EXPORT void _cpuid(cpuid_regs_t* regs);
}