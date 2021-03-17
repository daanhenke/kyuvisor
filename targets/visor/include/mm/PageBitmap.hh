#pragma once

#include <stdint.h>

namespace kyu::mm
{
    class PageBitmap
    {
    public:
        PageBitmap(void* buffer = nullptr, uint64_t size = 0) noexcept;
        uint8_t GetBit(uint64_t index) noexcept;
        void SetBit(uint64_t index, uint8_t val) noexcept;

        uint64_t FindFirstUnset(uint64_t index, uint64_t count) noexcept;
        uint64_t GetSetLength(uint64_t index, uint64_t count) noexcept;
        uint64_t GetUnsetLength(uint64_t index, uint64_t count) noexcept;

    protected:
        uint8_t* bitmap_buffer;
        uint64_t bitmap_size;
    };
}