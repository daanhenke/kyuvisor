#pragma once

#include <stdint.h>

#include "asm/instructions.hh"

namespace kyu::asm64
{
    // Register definitions
    typedef union cr4_t
    {
        uint64_t as_uint64;

        struct
        {
            uint64_t vme: 1;
            uint64_t pm: 1;
            uint64_t tsd: 1;
            uint64_t de: 1;
            uint64_t pse: 1;
            uint64_t pae: 1;
            uint64_t mce: 1;
            uint64_t pge: 1;
            uint64_t pce: 1;
            uint64_t osfxsr: 1;
            uint64_t osxmmexcpt: 1;
            uint64_t ump: 1;
            uint64_t la57: 1;
            uint64_t vmxe: 1;
            uint64_t smxe: 1;
            uint64_t fsgsbase: 1;
            uint64_t pcide: 1;
            uint64_t osxsave: 1;
            uint64_t smep: 1;
            uint64_t smap: 1;
            uint64_t pke: 1;
        };
    } cr4_t;

    // Base function definitions for reading and writing to registers, should not get called ever!
    template <typename T> T read() noexcept {}
    template <typename T> void write(T value) noexcept {}

    // Control registers
    template <> inline cr4_t read() noexcept { return cr4_t { _read_cr4() }; }
    template <> inline void write(cr4_t value) noexcept { _write_cr4(value.as_uint64); }
}