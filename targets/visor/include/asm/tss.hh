#pragma once

#include <stdint.h>

#define SEGMENT_DESCRIPTOR_TYPE_TSS_AVAILABLE   0x00000009

namespace kyu
{

#pragma pack(push, 1)
    typedef struct _task_state_segment_t
    {
        uint32_t reserved0;
        uint64_t rsp0;
        uint64_t rsp1;
        uint64_t rsp2;
        uint64_t reserved1;
        uint64_t ist[7];
        uint64_t reserved3;
        uint16_t reserved4;
        uint16_t ioMapBaseAddress;
    } task_state_segment_t;
#pragma pack(pop)

typedef union
{
    uint16_t flags;

    struct
    {
        uint16_t request_privilege_level : 2;
        uint16_t table : 1;
        uint16_t idx : 13;
    };
} segment_selector_t;


}
