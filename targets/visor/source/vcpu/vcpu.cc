#include "vcpu.hh"
#include "config.hh"

namespace kyu
{
    VirtualizedCPU::VirtualizedCPU()
    {

    }

    bool VirtualizedCPU::Start()
    {
        //config->LoaderFunctions.PrintString("Hello from cpu start\n");
        return true;
    }

    VirtualizedCPU** cpus;
}