#include "vcpu/VirtualizedCPU.hh"
#include "config.hh"
#include "asm/registers.hh"
#include "barelib/memory.hh"
#include "mm/MemoryAllocator.hh"

namespace kyu
{
    VirtualizedCPU::VirtualizedCPU()
    {
        state = CPUState::Disabled;

        memset(&vmcs, 0, sizeof(vmcs_t));
        memset(&vmxon, 0, sizeof(vmcs_t));
    }

    bool VirtualizedCPU::LoadVMXON()
    {
        auto loaderFuncs = config->LoaderFunctions;

        loaderFuncs.PrintString("Setting control registers...\n");
        FixControlRegisters();

        auto vmx_basic = asm64::msr::read<asm64::msr::vmx_basic_t>();

        vmxon.revision_id = vmx_basic.vmcs_revision_id;
        loaderFuncs.PrintString("Setting vmxon region...\n");
        loaderFuncs.PrintHex((uint64_t) vmxon.revision_id);
        loaderFuncs.PrintString("\n");
        auto res = asm64::_vmx_on(&vmxon);
        loaderFuncs.PrintHex(res);
        loaderFuncs.PrintString("\n");

        state = CPUState::Initializing;

        loaderFuncs.PrintHex((uint64_t) asm64::_inv_vpid);
        loaderFuncs.PrintString("Invalidating vpid & ept caches...\n");
        asm64::inv_vpid(asm64::invvpid_type_t::all_contexts);

        return true;
    }

    bool VirtualizedCPU::LoadVMCS()
    {
        auto loaderFuncs = config->LoaderFunctions;

        auto vmx_basic = asm64::msr::read<asm64::msr::vmx_basic_t>();
        vmcs.revision_id = vmx_basic.vmcs_revision_id;

        return true;
    }

    bool VirtualizedCPU::EnterVMX()
    {
        SetGdt();
        return LoadVMXON();
    }

    bool VirtualizedCPU::Start()
    {
        auto loaderFuncs = config->LoaderFunctions;

        loaderFuncs.PrintString(" starting!!!\n");
        // This will be either 0 if Capture returns or whatever rax is on context.Restore
        auto result = CPUState::Disabled;// static_cast<CPUState>(context.Capture());
        
        if (result == CPUState::Disabled)
        {
            EnterVMX();
        }

        return true;
    }

    void VirtualizedCPU::FixControlRegisters()
    {
        using namespace asm64;

        cr0_t new_cr0;
        cr4_t new_cr4;

        auto current_cr0 = read<cr0_t>();
        auto current_cr4 = read<cr4_t>();

        auto cr0_orfield = msr::read<msr::vmx_cr0_fixed0_t>();
        auto cr0_andfield = msr::read<msr::vmx_cr0_fixed1_t>();

        auto cr4_orfield = msr::read<msr::vmx_cr4_fixed0_t>();
        auto cr4_andfield = msr::read<msr::vmx_cr4_fixed1_t>();

        new_cr0.as_uint64 = current_cr0.as_uint64 | cr0_orfield.as_uint64;
        new_cr4.as_uint64 = current_cr4.as_uint64 | cr4_orfield.as_uint64;

        new_cr0.as_uint64 &= cr0_andfield.as_uint64;
        new_cr4.as_uint64 &= cr4_andfield.as_uint64;

        if (new_cr0.as_uint64 != current_cr0.as_uint64) asm64::write(new_cr0);
        if (new_cr4.as_uint64 != current_cr4.as_uint64) asm64::write(new_cr4);
    }

    void VirtualizedCPU::SetGdt()
    {
        segment_descriptor_register_t current_gdtr;
        asm64::_store_gdt(&current_gdtr);

        gdt = (segment_descriptor_t*) mm::allocator.Allocate(current_gdtr.limit + sizeof(segment_descriptor_t));
        memcpy(gdt, (void*) current_gdtr.base_address, current_gdtr.limit);

        segment_descriptor_t tss_descriptor;
        memset(&tss_descriptor, 0, sizeof(tss_descriptor));
        tss_descriptor.present = 1;
        tss_descriptor.type = SEGMENT_DESCRIPTOR_TYPE_TSS_AVAILABLE;
        tss_descriptor.segment_limit_low = sizeof(tss) - 1;

        auto tss_address = reinterpret_cast<uint64_t>(&tss);
        tss_descriptor.base_address_low = (tss_address & 0xFFFF);
        tss_descriptor.base_address_middle = ((tss_address >> 16) & 0xFF);
        tss_descriptor.base_address_high = ((tss_address >> 24) & 0xFF);
        tss_descriptor.base_address_upper = ((tss_address >> 32) & 0xFFFFFFFF);

        segment_descriptor_register_t new_gdtr;
        new_gdtr.limit = current_gdtr.limit + sizeof(segment_descriptor_t);
        new_gdtr.base_address = (uint64_t) gdt;

        segment_selector_t tr;
        tr.flags = 0;
        tr.idx = (current_gdtr.limit + 1ull) / sizeof(segment_descriptor_32_t);
        config->LoaderFunctions.PrintHex(tr.idx);

        auto gdt32 = reinterpret_cast<segment_descriptor_32_t*>(gdt);
        auto tss_descriptor_target = reinterpret_cast<segment_descriptor_t*>(&gdt32[tr.idx]);
        *tss_descriptor_target = tss_descriptor;

        asm64::_load_gdt(&new_gdtr);
        asm64::_load_tr(&tr);
    }

    VirtualizedCPU** cpus;
}
