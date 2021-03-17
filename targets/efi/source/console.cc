#include "console.hh"
#include "graphics.hh"

#include "barelib/string.hh"

#include <stdarg.h>

namespace kyu::console
{
    int current_x = 0;
    int current_y = 0;
    char cells[CONSOLE_WIDTH * CONSOLE_HEIGHT];

    void Reset()
    {
        current_x = 0;
        current_y = 0;

        for (int i = 0; i < CONSOLE_WIDTH * CONSOLE_HEIGHT; i++) cells[i] = '\0';
    }

    char GetCell(int x, int y)
    {
        return cells[y * CONSOLE_WIDTH + x];
    }

    void PrintHex(uint64_t number)
    {
        const char* chars = "0123456789ABCDEF";
        char buff[sizeof(uint64_t) * 2 + 1];
        buff[sizeof(uint64_t) * 2] = L'\0';
        uint64_t index = sizeof(uint64_t) * 2 - 1;

        for (int i = 0; i < sizeof(uint64_t) * 2; i++) buff[i] = '0';

        while (number > 0)
        {
            uint8_t lowest_nibble = number & 0xF;
            buff[index] = chars[lowest_nibble];
            number = number >> 4;
            index--;
        }

        PrintString(buff);
    }

    void PrintString(const char* string)
    {
        for (int i = 0; i < string_length(string); i++)
        {
            auto chr = string[i];

            if (chr == '\n')
            {
                current_x = 0;
                current_y++;
            }
            else if (chr == '\t')
            {
                current_x += 4 - (current_x % 4);
            }
            else
            {
                cells[current_y * CONSOLE_WIDTH + current_x] = chr;
                current_x++;
            }

            if (current_x >= CONSOLE_WIDTH)
            {
                current_x = 0;
                current_y++;
            }
        }

        graphics::DrawConsole();
    }
}