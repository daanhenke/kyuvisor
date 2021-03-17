#include "log.hh"

#include "efi/system-table.hh"

namespace kyu::log
{
    void String(wchar_t* str)
    {
        kyu::efi::system_table->console_out->output_string(kyu::efi::system_table->console_out, str);
    }

    void Char(wchar_t msg)
    {
        wchar_t buff[2];
        buff[0] = msg;
        buff[1] = L'\0';
        String(buff);
        String(L"\r\n");
    }

    void HexNumber(uint64_t number)
    {
        const wchar_t* chars = L"0123456789ABCDEF";
        wchar_t buff[sizeof(uint64_t) * 2 + 1];
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

        String(buff);
        String(L"\r\n");
    }
}