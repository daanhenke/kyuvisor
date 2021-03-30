#include "mm/MemoryAllocator.hh"
#include "config.hh"
#include "utilities.hh"

#include <cmath>

namespace kyu::mm
{
    MemoryAllocator::MemoryAllocator() noexcept : bitmap {}
    {

    }

    void MemoryAllocator::Initialize(void* base, uint64_t size) noexcept
    {
        region_base = base;
        region_size = free_size = size;
        used_size = 0;

        config->LoaderFunctions.PrintString("MM using base: ");
        config->LoaderFunctions.PrintHex((uint64_t) base);
        config->LoaderFunctions.PrintString("\n");

        previous_index = 0;
        bitmap = PageBitmap(region_base, size);
        lengthmap = (uint16_t*) ((uint64_t) region_base + (size / 8 + size % 8 != 0));
        
        // Allocate the space for our bitmap & lengthmap so it doesn't get used
        Allocate(size / 8 + (size % 8 != 0));
        Allocate(sizeof(uint16_t) * (size * 8));
    }

    void* MemoryAllocator::Allocate(uint64_t size) noexcept
    {
        return AllocatePages(utilities::bytes_to_pages(size));
    }

    void* MemoryAllocator::AllocatePages(uint64_t pages) noexcept
    {
        config->LoaderFunctions.PrintString("Allocating ");
        config->LoaderFunctions.PrintHex(pages);
        config->LoaderFunctions.PrintString(" pages!\n");

        previous_index = bitmap.FindFirstUnset(previous_index, pages);
        if (previous_index == -1)
        {
            previous_index = bitmap.FindFirstUnset(0, pages);

            if (previous_index == -1)
            {
                return nullptr;
            }
        }

        config->LoaderFunctions.PrintString("At ");
        config->LoaderFunctions.PrintHex(previous_index);
        config->LoaderFunctions.PrintString(" !\n");

        for (int i = 0; i < pages; i++) bitmap.SetBit(previous_index + i, 1);
        lengthmap[previous_index] = pages;

        return (void*) (((uint64_t) region_base) + previous_index * 4096);
    }

    MemoryAllocator allocator {};
}

#include <cstddef>
void* operator new(size_t size)
{
    return kyu::mm::allocator.Allocate(size);
}

void operator delete(void* address)
{

}