#include <stdint.h>
#include "config.hh"
#include "mm/MemoryAllocator.hh"
#include "asm/instructions.hh"

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
    kyu::mm::allocator.Initialize(config->PreparedMemoryRegion, config->PreparedMemorySize);
    
    kyu::asm64::cpuid_regs_t reggies;
    reggies.eax = 0;
    kyu::asm64::_cpuid(&reggies);
    char meme[13];
    meme[12] = 0;
    for (int i = 0; i < 4; i++)
    {
        meme[i] = reggies.as_string[i + 4];
        meme[i + 4] = reggies.as_string[i + 12];
        meme[i + 8] = reggies.as_string[i + 8];
    }
    config->LoaderFunctions.PrintString("Vendor according to cpuid: ");
    config->LoaderFunctions.PrintString(meme);
    config->LoaderFunctions.PrintString("\n");
    config->LoaderFunctions.PrintHex(reggies.ebx);
    config->LoaderFunctions.PrintString("\n");
    config->LoaderFunctions.PrintHex(reggies.edx);
    config->LoaderFunctions.PrintString("\n");
    config->LoaderFunctions.PrintHex(reggies.ecx);
    config->LoaderFunctions.PrintString("\n");
    
    return (uint64_t) config->MemeResult;
}

kyu::pub::HypervisorStartConfig* kyu::config = nullptr;