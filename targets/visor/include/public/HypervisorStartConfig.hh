#pragma once

#include <stdint.h>

namespace kyu::pub
{
    typedef __attribute__((sysv_abi)) void (*core_cb_t)() noexcept;

    typedef void (*LoaderPrintString)(const char* string) noexcept __attribute__((sysv_abi));
    typedef void (*LoaderPrintHex)(uint64_t number) noexcept __attribute__((sysv_abi));
    typedef uint64_t (*LoaderCPUCount)() noexcept __attribute__((sysv_abi));
    typedef uint64_t (*LoaderCPUCurrent)() noexcept __attribute__((sysv_abi));
    typedef void (*LoaderCPUExecuteOnAllCores)(core_cb_t callback) noexcept __attribute__((sysv_abi));

    typedef struct _LoaderSharedFunctions
    {
         LoaderPrintString PrintString;
         LoaderPrintHex PrintHex;
         LoaderCPUCount CPUCount;
         LoaderCPUCurrent CPUCurrent;
         LoaderCPUExecuteOnAllCores CPUExecuteOnAllCores;
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