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

        auto vmx_basic = asm64::msr::read<asm64::msr::vmx_basic_t>();
        if (
            ! vmx_basic.true_controls
        )
        {
            return false;
        }
        
        auto vmx_ept_cap = asm64::msr::read<asm64::msr::vmx_ept_vpid_cap_t>();
        if (
            ! vmx_ept_cap.page_walk_length_4
        )
        {
            return false;
        }

        return true;
    }
}