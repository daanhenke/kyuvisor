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

        if (! cpuid.vmx && false)
        {
            return false;
        }

        auto cr4 = asm64::read<asm64::cr4_t>();
        if (cr4.vmxe)
        {
            return false;
        }

        config->LoaderFunctions.PrintString("reading msr\n");

        auto msr = asm64::_read_msr(0x00000480);
        config->LoaderFunctions.PrintHex(msr);

        return true;
    }
}