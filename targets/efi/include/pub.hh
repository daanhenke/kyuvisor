#pragma once

#include <stdint.h>
#include "public/HypervisorStartConfig.hh"

bool InitializeServices();

void LoaderPrintString(const char* string) noexcept __attribute__((sysv_abi));
void LoaderPrintHex(uint64_t number) noexcept __attribute__((sysv_abi));

uint64_t LoaderCPUCount() noexcept __attribute__((sysv_abi));
uint64_t LoaderCPUCurrent() noexcept __attribute__((sysv_abi));
void LoaderCPUExecuteOnAllCores(kyu::pub::core_cb_t callback) noexcept __attribute__((sysv_abi));
