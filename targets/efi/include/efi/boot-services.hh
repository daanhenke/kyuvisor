#pragma once

#include "efi/core.hh"
#include "efi/protocol/macro.hh"

#define EfiReservedMemoryType       0x00000000
#define EfiLoaderCode               0x00000001
#define EfiLoaderData               0x00000002
#define EfiBootServicesCode         0x00000003
#define EfiBootServicesData         0x00000004
#define EfiRuntimeServicesCode      0x00000005
#define EfiRuntimeServicesData      0x00000006
#define EfiConventionalMemory       0x00000007
#define EfiUnusableMemory           0x00000008
#define EfiACPIReclaimMemory        0x00000009
#define EfiACPIMemoryNVS            0x0000000a
#define EfiMemoryMappedIO           0x0000000b
#define EfiMemoryMappedIOPortSpace  0x0000000c
#define EfiPalCode                  0x0000000d
#define EfiPersistentMemory         0x0000000e
#define EfiMaxMemoryType            0x0000000e

namespace kyu::efi
{
    typedef enum _locate_search_type_t
    {
        all_handles,
        by_register_notify,
        by_protocol
    } locate_search_type_t;

    typedef enum _allocate_type_t
    {
        allocate_any_pages,
        allocate_max_address,
        allocate_address,
        max_allocate_type
    } allocate_type_t;

    PROTOCOL_METHOD(protocol_padding_t, ())
    PROTOCOL_METHOD(bs_allocate_pages_t, (allocate_type_t allocate_type, uint64_t memory_type, uint64_t pages, physical_address_t* memory));
    PROTOCOL_METHOD(bs_wait_for_event_t, (uint64_t number_of_events, event_t* events, uint64_t* index))
    PROTOCOL_METHOD(bs_handle_protocol_t, (handle_t handle, guid_t* protocol, void** interface))
    PROTOCOL_METHOD(bs_locate_handle_buffer_t, (locate_search_type_t search_type, guid_t* protocol, void* search_key, uint64_t* buffer_size, handle_t** buffer))

    typedef struct _boot_services_t
    {
        table_header_t header;
        protocol_padding_t raise_tpl;
        protocol_padding_t restore_tpl;
        bs_allocate_pages_t allocate_pages;
        protocol_padding_t free_pages;
        protocol_padding_t get_memory_map;
        protocol_padding_t allocate_pool;
        protocol_padding_t free_pool;
        protocol_padding_t create_event;
        protocol_padding_t set_timer;
        bs_wait_for_event_t wait_for_event;
        protocol_padding_t signal_event;
        protocol_padding_t close_event;
        protocol_padding_t check_event;
        protocol_padding_t install_protocol_interface;
        protocol_padding_t reinstall_protocol_interface;
        protocol_padding_t uninstall_protocol_interface;
        bs_handle_protocol_t handle_protocol;
        void*               reserved;
        protocol_padding_t register_protocol_notify;
        protocol_padding_t locate_handle;
        protocol_padding_t locate_device_path;
        protocol_padding_t install_configuration_table;
        protocol_padding_t load_image;
        protocol_padding_t start_image;
        protocol_padding_t exit;
        protocol_padding_t unload_image;
        protocol_padding_t exit_boot_services;
        protocol_padding_t get_next_monotonic_count;
        protocol_padding_t stall;
        protocol_padding_t set_watchdog_timer;
        protocol_padding_t connect_controller;
        protocol_padding_t disconnect_controller;
        protocol_padding_t open_protocol;
        protocol_padding_t close_protocol;
        protocol_padding_t open_protocol_information;
        protocol_padding_t protocols_per_handle;
        bs_locate_handle_buffer_t locate_handle_buffer;
        protocol_padding_t locate_protocol;
        protocol_padding_t install_multiple_protocol_interfaces;
        protocol_padding_t uninstall_multiple_protocol_interfaces;
        protocol_padding_t calculate_crc32;
        protocol_padding_t copy_mem;
        protocol_padding_t set_mem;
        protocol_padding_t create_event_ex;
    } boot_services_t;
}