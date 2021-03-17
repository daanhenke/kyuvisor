#pragma once

#include "efi/protocol/macro.hh"
#include "efi/protocol/file.hh"

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID        {0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

namespace kyu::efi::protocol
{
    struct _simple_filesystem_protocol_t;

    PROTOCOL_METHOD(sfsp_open_volume_t, (struct _simple_filesystem_protocol_t* _this, file_protocol_t** root))

    typedef struct _simple_filesystem_protocol_t
    {
        uint64_t revision;
        sfsp_open_volume_t open_volume;
    } simple_filesystem_protocol_t;
}