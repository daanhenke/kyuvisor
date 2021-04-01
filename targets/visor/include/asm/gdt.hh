#pragma once

#include <stdint.h>

namespace kyu
{


typedef struct _segment_descriptor_t
{
    uint16_t segment_limit_low;
    uint16_t base_address_low;
    union
    {
        struct
        {
            uint32_t base_address_middle : 8;
            uint32_t type : 4;
            uint32_t descriptor_type : 1;
            uint32_t descriptor_privilege_level : 2;
            uint32_t present : 1;
            uint32_t segment_limit_high : 4;
            uint32_t system : 1;
            uint32_t long_mode : 1;
            uint32_t default_big : 1;
            uint32_t granularity : 1;
            uint32_t base_address_high : 8;
        };

        uint32_t flags;
    };
    uint32_t base_address_upper;
    uint32_t must_be_zero;
} segment_descriptor_t;

typedef struct
{
    uint16_t segment_limit_low;
    uint16_t base_address_low;

    union
    {
        struct
        {
            uint32_t base_address_middle : 8;
            uint32_t type : 4;
            uint32_t descriptor_type : 1;
            uint32_t descriptor_privilege_level : 2;
            uint32_t present : 1;
            uint32_t segment_limit_high : 4;
            uint32_t system : 1;
            uint32_t long_mode : 1;
            uint32_t default_big : 1;
            uint32_t granularity : 1;
            uint32_t base_address_high : 8;
       };

        uint32_t flags;
    };

} segment_descriptor_32_t;



#pragma pack(push, 1)
    typedef struct _segment_descriptor_register_t
    {
        uint16_t limit;
        uint64_t base_address;
    } segment_descriptor_register_t;

    typedef segment_descriptor_register_t gdtr_t;
#pragma pack(pop)
}
