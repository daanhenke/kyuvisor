#include <stdint.h>
#include "config.hh"
#include "mm/MemoryAllocator.hh"
#include "featurecheck.hh"

class AyLmao
{
public:
    AyLmao() noexcept
    {
        kyu::config->LoaderFunctions.PrintString("I got dynamically allocated!\n");
    }
};

extern "C" uint64_t entrypoint(kyu::pub::HypervisorStartConfig* config) noexcept
{
    kyu::config = config;
    auto funcs = config->LoaderFunctions;
    
    funcs.PrintString("Hello from Hypervisor!\n");
    kyu::mm::allocator.Initialize(config->PreparedMemoryRegion, config->PreparedMemorySize);
    
    if (! kyu::featurecheck::SupportsVMX())
    {
        funcs.PrintString("ERROR: This processor does not support VMX\n");
        return 1;
    }

    funcs.PrintString("Hypervisor support present b0ss\n");
    
    return (uint64_t) 0;
}

kyu::pub::HypervisorStartConfig* kyu::config = nullptr;