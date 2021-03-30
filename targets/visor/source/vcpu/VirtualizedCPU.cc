#include "vcpu/VirtualizedCPU.hh"
#include "config.hh"
#include "asm/registers.hh"

namespace kyu
{
    VirtualizedCPU::VirtualizedCPU()
    {
        state = CPUState::Disabled;
    }

    bool VirtualizedCPU::LoadVMXON()
    {
        FixControlRegisters();

        auto vmx_basic = asm64::msr::read<asm64::msr::vmx_basic_t>();

        vmxon.revision_id = vmx_basic.vmcs_revision_id;
        asm64::_vmx_on(&vmxon);

        state = CPUState::Initializing;

        asm64::inv_vpid(asm64::invvpid_type_t::all_contexts);

        return true;
    }

    bool VirtualizedCPU::EnterVMX()
    {
        return LoadVMXON();
    }

    bool VirtualizedCPU::Start()
    {
        // This will be either 0 if Capture returns or whatever rax is on context.Restore
        auto result = static_cast<CPUState>(context.Capture());
        
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

    VirtualizedCPU** cpus;
}
