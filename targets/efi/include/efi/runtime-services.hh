#pragma once

#include "efi/core.hh"

namespace kyu::efi
{
    typedef struct _runtime_services_t
    {
        table_header_t header;
    } runtime_services_t;
}