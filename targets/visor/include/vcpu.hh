#pragma once

namespace kyu
{
    class VirtualizedCPU
    {
    public:
        VirtualizedCPU();
        bool Start();
    };

    extern VirtualizedCPU** cpus;
}