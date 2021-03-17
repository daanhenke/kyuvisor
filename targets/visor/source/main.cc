#include <stdint.h>
#include "public/HypervisorStartConfig.hh"

extern "C" uint64_t entrypoint(kyu::pub::HypervisorStartConfig* config)
{
    const char* meme = "Hellooooooooooooooooooooooo!\n";
    config->LoaderFunctions.PrintString(meme);
    // config->LoaderFunctions.PrintString("Hello from Hypervisor!\n");
    // config->LoaderFunctions.PrintString("Hello from Hypervisor!\n");
    // config->LoaderFunctions.PrintString("Hello from Hypervisor!\n");
    // config->LoaderFunctions.PrintString("Hello from Hypervisor!\n");
    // config->LoaderFunctions.PrintString("Hello from Hypervisor!\n");
    // config->LoaderFunctions.PrintString("Hello from Hypervisor!\n");
    return (uint64_t) config->MemeResult;
}