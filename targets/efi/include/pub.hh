#pragma once

#include <stdint.h>

void LoaderPrintString(const char* string) noexcept __attribute__((sysv_abi));
void LoaderPrintHex(uint64_t number) noexcept __attribute__((sysv_abi));