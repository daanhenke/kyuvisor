#pragma once

#include <stdint.h>

namespace kyu::log
{
    void String(wchar_t* str);
    void Char(wchar_t msg);
    void HexNumber(uint64_t number);
}