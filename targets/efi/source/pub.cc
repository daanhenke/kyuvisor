#include "pub.hh"
#include "console.hh"

void LoaderPrintString(const char* string) noexcept
{
    kyu::console::PrintString(string);
}

void LoaderPrintHex(uint64_t number) noexcept
{
    kyu::console::PrintHex(number);
}