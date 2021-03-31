#pragma once

#include "vcpu/CPUContext.hh"
#include "asm/vmcx.hh"

namespace kyu
{
    class VirtualizedCPU
    {
    public:
        enum CPUState
        {
            Disabled,
            Enabled,
            Initializing
        };

        VirtualizedCPU();
        bool Start();

        bool EnterVMX();
        bool LoadVMXON();
        void FixControlRegisters();

    protected:
        CPUState state;
        CPUContext context;

        vmcs_t vmxon;
        vmcs_t vmcs;
    };

    extern VirtualizedCPU** cpus;
}
