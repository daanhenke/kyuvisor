#pragma once

#include <stdint.h>

#include "asm/instructions.hh"

namespace kyu::asm64
{
    // Register definitions
    typedef union cr0_t
    {
        uint64_t as_uint64;

        struct
        {
            uint64_t protection_enable : 1;
            uint64_t monitor_coprocessor : 1;
            uint64_t emulate_fpu : 1;
            uint64_t task_switched : 1;
            uint64_t extension_type : 1;
            uint64_t numeric_error : 1;
            uint64_t reserved_1 : 10;
            uint64_t write_protect : 1;
            uint64_t reserved_2 : 1;
            uint64_t alignment_mask : 1;
            uint64_t reserved_3 : 10;
            uint64_t not_write_through : 1;
            uint64_t cache_disable : 1;
            uint64_t paging_enable : 1;
        };
    } cr0_t;

    typedef union cr4_t
    {
        uint64_t as_uint64;

        struct
        {
            uint64_t virtual_mode_extensions : 1;
            uint64_t protected_mode_virtual_interrupts : 1;
            uint64_t timestamp_disable : 1;
            uint64_t debugging_extensions : 1;
            uint64_t page_size_extensions : 1;
            uint64_t physical_address_extension : 1;
            uint64_t machine_check_enable : 1;
            uint64_t page_global_enable : 1;
            uint64_t performance_monitoring_counter_enable : 1;
            uint64_t os_fxsave_fxrstor_support : 1;
            uint64_t os_xmm_exception_support : 1;
            uint64_t usermode_instruction_prevention : 1;
            uint64_t reserved_1 : 1;
            uint64_t vmx_enable : 1;
            uint64_t smx_enable : 1;
            uint64_t reserved_2 : 1;
            uint64_t fsgsbase_enable : 1;
            uint64_t pcid_enable : 1;
            uint64_t os_xsave : 1;
            uint64_t reserved_3 : 1;
            uint64_t smep_enable : 1;
            uint64_t smap_enable : 1;
            uint64_t protection_key_enable : 1;
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
    template <> inline cr0_t read() noexcept { return cr0_t { _read_cr0() }; }
    template <> inline void write(cr0_t value) noexcept { _write_cr0(value.as_uint64); }

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

        struct vmx_cr0_fixed0_t : msr_out_t
        {
            static constexpr uint32_t __id = 0x486;
            using __type = asm64::cr0_t;
        };

        struct vmx_cr0_fixed1_t : msr_out_t
        {
            static constexpr uint32_t __id = 0x487;
            using __type = asm64::cr0_t;
        };

        struct vmx_cr4_fixed0_t : msr_out_t
        {
            static constexpr uint32_t __id = 0x488;
            using __type = asm64::cr4_t;
        };

        struct vmx_cr4_fixed1_t : msr_out_t
        {
            static constexpr uint32_t __id = 0x489;
            using __type = asm64::cr4_t;
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