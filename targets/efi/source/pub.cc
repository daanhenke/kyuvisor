#include "pub.hh"
#include "console.hh"

void LoaderPrintString(const char* string) noexcept
{
    kyu::console::PrintHex((uint64_t) string);
}