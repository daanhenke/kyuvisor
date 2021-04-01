#pragma once

#include "vcpu/CPUContext.hh"
#include "asm/vmcs.hh"
#include "asm/gdt.hh"
#include "asm/tss.hh"

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
        bool LoadVMCS();
        void FixControlRegisters();
        void SetGdt();

    protected:
        CPUState state;
        CPUContext context;

        segment_descriptor_t* gdt;
        task_state_segment_t tss;

        vmcs_t vmxon;
        vmcs_t vmcs;
    };

    extern VirtualizedCPU** cpus;
}
