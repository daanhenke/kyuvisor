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

    typedef union vmx_basic_t
    {
        uint64_t as_uint64;

        struct
        {
            uint64_t vmcs_revision_id : 31;
            uint64_t reserved_1 : 1;
            uint64_t vmcs_size_in_bytes : 13;
            uint64_t reserved_2 : 3;
            uint64_t vmcs_physical_address_width : 1;
            uint64_t dual_monitor : 1;
            uint64_t memory_type : 4;
            uint64_t ins_outs_vmexit_information : 1;
            uint64_t true_controls : 1;
        };
    } vmx_basic_t;

    typedef union vmx_ept_vpid_cap_t
    {
        uint64_t as_uint64;

        struct
        {
            uint64_t execute_only_pages : 1;
            uint64_t reserved_1 : 5;
            uint64_t page_walk_length_4 : 1;
            uint64_t reserved_2 : 1;
            uint64_t memory_type_uncacheable : 1;
            uint64_t reserved_3 : 5;
            uint64_t memory_type_write_back : 1;
            uint64_t reserved_4 : 1;
            uint64_t pde_2mb_pages : 1;
            uint64_t pdpte_1gb_pages : 1;
            uint64_t reserved_5 : 2;
            uint64_t invept : 1;
            uint64_t ept_accessed_and_dirty_flags : 1;
            uint64_t advanced_vmexit_ept_violations_information : 1;
            uint64_t reserved_6 : 2;
            uint64_t invept_single_context : 1;
            uint64_t invept_all_contexts : 1;
            uint64_t reserved_7 : 5;
            uint64_t invvpid : 1;
            uint64_t reserved_8 : 7;
            uint64_t invvpid_individual_address : 1;
            uint64_t invvpid_single_context : 1;
            uint64_t invvpid_all_contexts : 1;
            uint64_t invvpid_single_context_retain_globals : 1;
        };
    } vmx_ept_vpid_cap_t;

    // Base function definitions for reading and writing to registers, should not get called ever!
    template <typename T> T read() noexcept {}
    template <typename T> void write(T value) noexcept {}

    // Control registers
    template <> inline cr4_t read() noexcept { return cr4_t { _read_cr4() }; }
    template <> inline void write(cr4_t value) noexcept { _write_cr4(value.as_uint64); }

    namespace msr
    {
        // These structs are used to automatically resolve the msr id & resulting type of the read & write functions
        struct msr_out_t {};

        struct vmx_basic_t : msr_out_t
        {
            static constexpr uint32_t __id = 0x480;
            using __type = asm64::vmx_basic_t;
        };

        struct vmx_ept_vpid_cap_t : msr_out_t
        {
            static constexpr uint32_t __id = 0x48c;
            using __type = asm64::vmx_ept_vpid_cap_t;
        };

        // MSR register read & write functions
        template <typename T>
        inline auto read() noexcept { return typename T::__type { _read_msr(T::__id) }; }
    }
}