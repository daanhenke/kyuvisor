#pragma once

#include <stdint.h>

#define EFI_SUCCESS                 0x0000000000000000
#define EFI_ERR                     0x8000000000000000
#define EFI_CUSTOM_ERROR(code)      (EFI_ERR | 0x1000 | code)
#define EFI_BUFFER_TOO_SMALL        (EFI_ERR | 0x0000000000000005)

#define EFI_PAGE_MASK               0xFFF
#define EFI_PAGE_SHIFT		        12
#define EFI_PAGE_SIZE		        (1UL << EFI_PAGE_SHIFT)
#define EFI_SIZE_TO_PAGES(size)     (((size) >> EFI_PAGE_SHIFT) + (((size) & EFI_PAGE_MASK) ? 1 : 0))

namespace kyu::efi
{
    typedef wchar_t echar16_t;
    typedef uint8_t bool_t;

    typedef void* handle_t;
    typedef void* event_t;

    typedef uint64_t physical_address_t;
    typedef uint64_t status_t;

    typedef struct _guid_t
    {
        uint32_t data1;
        uint16_t data2;
        uint16_t data3;
        uint8_t data4[8];
    } guid_t;

    typedef struct _table_header_t
    {
        uint64_t signature;
        uint32_t revision;
        uint32_t size;
        uint32_t crc32;
        uint32_t reserved;
    } table_header_t;

    typedef struct _configuration_table_t
    {
        guid_t vendor_guid;
        void* vendor_table;
    } configuration_table_t;

    typedef struct _time_t
    {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t pad1;
        uint32_t nanosecond;
        int16_t timezone;
        uint8_t daylight;
        uint8_t pad2;
    } time_t;
}