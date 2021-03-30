#pragma once

#include "vcpu/CPUContext.hh"

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
    };

    extern VirtualizedCPU** cpus;
}
