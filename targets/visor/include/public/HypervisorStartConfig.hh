#pragma once

#include <stdint.h>

namespace kyu::pub
{
    typedef void (*LoaderPrintString)(const char* string) noexcept __attribute__((sysv_abi));
    typedef void (*LoaderPrintHex)(uint64_t number) noexcept __attribute__((sysv_abi));

    typedef struct
    {
         LoaderPrintString PrintString;
         LoaderPrintHex PrintHex;
    } LoaderSharedFunctions;

    typedef struct HypervisorStartConfig
    {
        uint64_t LoaderVersion;
        LoaderSharedFunctions LoaderFunctions;
        void* PreparedMemoryRegion;
        uint64_t PreparedMemorySize;
        uint64_t MemeResult;
    } HypervisorStartConfig;
}