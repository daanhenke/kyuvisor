#include "pub.hh"
#include "console.hh"
#include "efi/system-table.hh"
#include "efi/protocol/multi-processor.hh"

kyu::efi::protocol::mp_services_protocol_t *mp_services;

bool InitializeServices()
{
    using namespace kyu::efi;

    guid_t mp_guid = EFI_MULTI_PROCESSING_PROTOCOL_GUID;
    auto result = system_table->boot_services->locate_protocol(&mp_guid, nullptr, (void**) &mp_services);

    if (result != 0)
    {
        return false;
    }

    return true;
}

void LoaderPrintString(const char* string) noexcept
{
    kyu::console::PrintString(string);
}

void LoaderPrintHex(uint64_t number) noexcept
{
    kyu::console::PrintHex(number);
}

uint64_t LoaderCPUCount() noexcept
{
    uint64_t unused;
    uint64_t res;
    
    mp_services->get_number_of_processors(mp_services, &unused, &res);
    
    return res;
}

uint64_t LoaderCPUCurrent() noexcept
{
    uint64_t res;
    
    mp_services->whoami(mp_services, &res);
    
    return res;
}

void StartupAllApsCallback(void* argument)
{
    auto cb = reinterpret_cast<kyu::pub::core_cb_t>(argument);
    cb();
}

void LoaderCPUExecuteOnAllCores(kyu::pub::core_cb_t callback) noexcept
{
    callback();

    if (LoaderCPUCount() == 1) return;
    kyu::console::PrintString("starting other cores\n");

    mp_services->startup_all_aps(mp_services, (kyu::efi::protocol::ap_procedure_t) StartupAllApsCallback, 1, nullptr, 0, reinterpret_cast<void*>(callback), nullptr);
}

extern "C" void* memcpy(void* dst, const void* src, unsigned int cnt)
{
    char *pszDest = (char *)dst;
    const char *pszSource =( const char*)src;
    if((pszDest!= nullptr) && (pszSource!= nullptr))
    {
        while(cnt) //till cnt
        {
            //Copy byte by byte
            *(pszDest++)= *(pszSource++);
            --cnt;
        }
    }
    return dst;
}

extern "C" int strcmp(const char* s1, const char* s2)
{
    while (*s1 == *s2 && *s1 != '\0')
    {
        s1++;
        s2++;
    }

    return (*(unsigned char*) s1) - (*(unsigned char*) s2);
}

