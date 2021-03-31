#pragma once

#define CONSOLE_WIDTH 48
#define CONSOLE_HEIGHT 30

#include <stdint.h>

namespace kyu::console
{
    void Reset();
    char GetCell(int x, int y);

    bool GetFullScreenRefresh();
    void SetFullScreenRefresh(bool val);

    void PrintHex(uint64_t number);
    void PrintString(const char* string);
}
