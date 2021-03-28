#include <stdint.h>
#include "config.hh"
#include "mm/MemoryAllocator.hh"
#include "featurecheck.hh"
#include "vcpu.hh"

void StartCPU() noexcept
{
    using namespace kyu;
    auto funcs = config->LoaderFunctions;

    auto core_index = funcs.CPUCurrent();
    
    funcs.PrintString("Hello from core ");
    funcs.PrintHex(core_index);
    funcs.PrintString("!\n");

    auto cpu = cpus[core_index];
    funcs.PrintString("Starting cpu @ ");
    funcs.PrintHex((uint64_t) cpu);
    funcs.PrintString("\n");
}

extern "C" uint64_t entrypoint(kyu::pub::HypervisorStartConfig* config) noexcept
{
    kyu::config = config;
    auto funcs = config->LoaderFunctions;

    using namespace kyu;
    
    funcs.PrintString("Hello from Hypervisor!\n");
    mm::allocator.Initialize(config->PreparedMemoryRegion, config->PreparedMemorySize);
    
    funcs.PrintString("Checking support for VMX\n");
    if (! featurecheck::SupportsVMX())
    {
        funcs.PrintString("ERROR: This processor does not support VMX\n");
        return 1;
    }

    funcs.PrintString("Hypervisor support present b0ss\n");

    auto cpu_count = funcs.CPUCount();

    funcs.PrintString("CPU count: ");
    funcs.PrintHex(cpu_count);
    funcs.PrintString("\n");

    cpus = (VirtualizedCPU**) mm::allocator.Allocate(sizeof(VirtualizedCPU*) * cpu_count);
    for (uint64_t i = 0; i < cpu_count; i++)
    {
        cpus[i] = new VirtualizedCPU();
    }

    funcs.PrintString("Starting cpu cores!\n");
    funcs.CPUExecuteOnAllCores(reinterpret_cast<kyu::pub::core_cb_t>(StartCPU));
    return 0;
}

kyu::pub::HypervisorStartConfig* kyu::config = nullptr;