#include "config.hh"
#include "featurecheck.hh"
#include "asm/registers.hh"

namespace kyu::featurecheck
{
    bool SupportsVMX()
    {
        asm64::cpuid_regs_t cpuid;

        cpuid.eax = 1;
        asm64::_cpuid(&cpuid);

        if (! cpuid.vmx)
        {
            return false;
        }

        auto cr4 = asm64::read<asm64::cr4_t>();
        if (cr4.vmxe)
        {
            return false;
        }

        config->LoaderFunctions.PrintString("Reading msr: ");

        auto msr = asm64::read<asm64::msr::vmx_basic_t>();
        config->LoaderFunctions.PrintString("msr: ");
        config->LoaderFunctions.PrintHex(msr.value);
        config->LoaderFunctions.PrintString("\n");

        return true;
    }
}