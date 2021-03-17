#include "mm/PageBitmap.hh"

namespace kyu::mm
{
    PageBitmap::PageBitmap(void* buffer, uint64_t size) noexcept
    {
        bitmap_buffer = (uint8_t*) buffer;
        bitmap_size = size;
    }
    
    uint8_t PageBitmap::GetBit(uint64_t index) noexcept
    {
        return (bitmap_buffer[index / 8] >> (index % 8)) & 1;
    }

    void PageBitmap::SetBit(uint64_t index, uint8_t val) noexcept
    {
        val != 0 ? (bitmap_buffer[index / 8] |= (1 << index % 8)) : (bitmap_buffer[index / 8] &= (~(1) << (index % 8)));
    }
    
    uint64_t PageBitmap::FindFirstUnset(uint64_t index, uint64_t count) noexcept
    {
        uint64_t current_index = index;

        while (current_index + count < bitmap_size)
        {
            current_index += GetSetLength(current_index, 1000);
            uint64_t res_length = GetUnsetLength(current_index, count);

            if (res_length >= count) return current_index;

            current_index += res_length;
        }

        return -1;
    }
 
    uint64_t PageBitmap::GetSetLength(uint64_t index, uint64_t count) noexcept
    {
        if (index >= bitmap_size) return 0;

        uint64_t length = 0;
        while (length < count)
        {
            if (GetBit(index + length) != 0) length++; else break;
        }

        return length;
    }

    uint64_t PageBitmap::GetUnsetLength(uint64_t index, uint64_t count) noexcept
    {
        if (index >= bitmap_size) return 0;

        uint64_t length = 0;
        while (length < count)
        {
            if (GetBit(index + length) == 0) length++; else break;
        }

        return length;
    }
}