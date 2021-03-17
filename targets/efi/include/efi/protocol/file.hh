#pragma once

#include "efi/protocol/macro.hh"

#define EFI_FILE_INFO_ID                {0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_FILE_SYSTEM_INFO_ID         {0x09576e93, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}} 
#define EFI_FILE_SYSTEM_VOLUME_LABEL_ID {0xdb47d7d3, 0xfe81, 0x11d3, {0x9a, 0x35, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}

#define EFI_FILE_MODE_READ      0x0000000000000001
#define EFI_FILE_MODE_WRITE     0x0000000000000002
#define EFI_FILE_MODE_CREATE    0x8000000000000000

#define EFI_FILE_READ_ONLY  0x0000000000000001
#define EFI_FILE_HIDDEN     0x0000000000000002
#define EFI_FILE_SYSTEM     0x0000000000000004
#define EFI_FILE_RESERVED   0x0000000000000008
#define EFI_FILE_DIRECTORY  0x0000000000000010
#define EFI_FILE_ARCHIVE    0x0000000000000020
#define EFI_FILE_VALID_ATTR 0x0000000000000037

namespace kyu::efi::protocol
{
    typedef struct _file_info_t
    {
        uint64_t size;
        uint64_t file_size;
        uint64_t physical_size;
        time_t create_time;
        time_t last_access_time;
        time_t modification_time;
        uint64_t attribute;
        echar16_t file_name[128];
    } file_info_t;

    typedef struct _filesystem_info_t
    {
        uint64_t size;
        bool_t read_only;
        uint64_t volume_size;
        uint64_t free_space;
        uint32_t block_size;
        echar16_t volume_label[128];
    } filesystem_info_t;

    typedef struct _filesystem_volume_label_t
    {
        echar16_t volume_label[128];
    } filesystem_volume_label_t;

    struct _file_protocol_t;

    PROTOCOL_METHOD(fp_open_t, (struct _file_protocol_t* _this, struct _file_protocol_t** new_handle, echar16_t* filename, uint64_t open_mode, uint64_t attributes))
    PROTOCOL_METHOD(fp_read_t, (struct _file_protocol_t* _this, uint64_t* buffer_size, void* buffer))
    PROTOCOL_METHOD(fp_get_info_t, (struct _file_protocol_t* _this, guid_t* information_type, uint64_t* buffer_size, void* buffer))

    typedef struct _file_protocol_t
    {
        uint64_t revision;
        fp_open_t open;
        fp_open_t close;
        fp_open_t delete_;
        fp_read_t read;
        fp_open_t write;
        fp_open_t get_position;
        fp_open_t set_position;
        fp_get_info_t get_info;
        fp_open_t set_info;
        fp_open_t flush;
        fp_open_t open_ex;
        fp_open_t read_ex;
        fp_open_t write_ex;
        fp_open_t flush_ex;
    } file_protocol_t;
}