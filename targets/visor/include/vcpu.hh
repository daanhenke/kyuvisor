#pragma once

namespace kyu
{
    class VirtualizedCPU
    {
    public:
        VirtualizedCPU();
        bool Start();

        // Helper functions
        void FixControlRegisters();
    };

    extern VirtualizedCPU** cpus;
}