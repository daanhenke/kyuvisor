#pragma once

#include <stdint.h>

namespace kyu::utilities
{
    template<typename T>
    constexpr inline uint64_t bytes_to_pages(T bytes, uint8_t page_shift = 12) noexcept
    {
        return (bytes >> page_shift) + (~(bytes & ~((1 << page_shift) - 1)) != 0);
    } 
}