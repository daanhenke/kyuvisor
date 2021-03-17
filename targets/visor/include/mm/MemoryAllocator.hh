#pragma once

#include <stdint.h>

#include "mm/PageBitmap.hh"

namespace kyu::mm
{
    class MemoryAllocator
    {
    public:
        MemoryAllocator() noexcept;
        void Initialize(void* base, uint64_t size) noexcept;
        void* Allocate(uint64_t size) noexcept;
        void* AllocatePages(uint64_t count) noexcept;

    protected:
        void* region_base;

        uint64_t region_size;
        uint64_t free_size;
        uint64_t used_size;

        uint64_t previous_index;
        PageBitmap bitmap;
        uint16_t* lengthmap;
    };

    extern MemoryAllocator allocator;
}