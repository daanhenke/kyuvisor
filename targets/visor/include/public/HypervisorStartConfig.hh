#pragma once

#include <stdint.h>

namespace kyu::pub
{
    typedef void (*LoaderPrintString)(const char* string) noexcept __attribute__((sysv_abi));

    typedef struct
    {
         LoaderPrintString PrintString;
    } LoaderSharedFunctions;

    typedef struct HypervisorStartConfig
    {
        uint64_t LoaderVersion;
        LoaderSharedFunctions LoaderFunctions;
        uint64_t MemeResult;
    } HypervisorStartConfig;
}