#include "console.hh"
#include "graphics.hh"

#include "barelib/string.hh"

#include <stdarg.h>

namespace kyu::console
{
    int current_x = 0;
    int current_y = 0;
    char cells[CONSOLE_WIDTH * CONSOLE_HEIGHT];
    bool full_screen_refresh = true;

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

    bool GetFullScreenRefresh()
    {
        return full_screen_refresh;
    }

    void SetFullScreenRefresh(bool val)
    {
        full_screen_refresh = val;
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

    void ScrollUp(uint64_t count = 1)
    {
        while (count > 0)
        {
            count--;

            for (uint64_t y = 0; y < CONSOLE_HEIGHT - 1; y++)
            {
                for (uint64_t x = 0; x < CONSOLE_WIDTH; x++)
                {
                    cells[y * CONSOLE_WIDTH + x] = cells[(y + 1) * CONSOLE_WIDTH + x];
                }
            }

            for (uint64_t x = 0; x < CONSOLE_WIDTH; x++)
            {
                cells[(CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH + x] = '\0';
            }

            current_y--;
        }

        SetFullScreenRefresh(true);
    }

    void PrintString(const char* string)
    {
        for (int i = 0; i < strlen(string); i++)
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

            if (current_y >= CONSOLE_HEIGHT)
            {
                ScrollUp(current_y + 1 - CONSOLE_HEIGHT + 4);
            }
        }

        graphics::DrawConsole();
    }
}
