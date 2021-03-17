#include <stdint.h>
#include "config.hh"
#include "mm/MemoryAllocator.hh"

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
    
    config->LoaderFunctions.PrintString("Hello from Hypervisor!\n");

    config->LoaderFunctions.PrintString("Initializing memory allocator @");
    config->LoaderFunctions.PrintHex((uint64_t) config->PreparedMemoryRegion);
    config->LoaderFunctions.PrintString(" with size of ");
    config->LoaderFunctions.PrintHex(config->PreparedMemorySize * 4096);
    config->LoaderFunctions.PrintString(" bytes\n");
    
    kyu::mm::allocator.Initialize(config->PreparedMemoryRegion, config->PreparedMemorySize);
    
    auto test = new AyLmao();
    
    return (uint64_t) config->MemeResult;
}

kyu::pub::HypervisorStartConfig* kyu::config = nullptr;