#include "vcpu/VirtualizedCPU.hh"
#include "config.hh"
#include "asm/registers.hh"

namespace kyu
{
    VirtualizedCPU::VirtualizedCPU()
    {
        state = CPUState::Disabled;
    }

    bool LoadVMXON()
    {
        

        auto vmx_basic = asm64::msr::read<asm64::msr::vmx_basic_t>();
    }

    bool EnterVMX()
    {
        return LoadVMXON();
    }

    bool VirtualizedCPU::Start()
    {
        // This will be either 0 if Capture returns or whatever rax is on context.Restore
        auto result = static_cast<CPUState>(context.Capture());
        
        if (result == CPUState::Disabled)
        {
            
        }

        return true;
    }

    VirtualizedCPU** cpus;
}