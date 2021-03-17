#pragma once

#include <stdint.h>

namespace barelib::farbfeld
{
    typedef struct _header_t
    {
        uint64_t magic;
        uint8_t width[4];
        uint8_t height[4];
    } header_t;
}