#pragma once

#include <stdint.h>

namespace kyu
{
    typedef struct alignas(4096) _vmcs_t
    {
        uint32_t revision_id;
    } vmcs_t;
}
