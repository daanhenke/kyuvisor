#pragma once

#include <stdint.h>

namespace kyu
{
    struct alignas(4096) vmcs_t
    {
        uint32_t revision_id;
    };
}
