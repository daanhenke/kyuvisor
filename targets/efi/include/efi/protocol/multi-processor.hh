#pragma once

#include "efi/protocol/macro.hh"
#include "efi/core.hh"

#define EFI_MULTI_PROCESSING_PROTOCOL_GUID        { 0x3fdda605, 0xa76e, 0x4f46, {0xad, 0x29, 0x12, 0xf4, 0x53, 0x1b, 0x3d, 0x08} };

namespace kyu::efi::protocol
{
    typedef void (*ap_procedure_t)(void *buffer);
 
    struct _mp_services_protocol_t;

    PROTOCOL_METHOD(mp_get_number_of_processors, (struct _mp_services_protocol_t* _this, uint64_t *number_of_processors, uint64_t *number_of_enabled_processors))
    PROTOCOL_METHOD(mp_startup_all_aps, (struct _mp_services_protocol_t* _this, ap_procedure_t procedure, bool_t single_thread, event_t wait_event, uint64_t timeout_in_microseconds, void* procedure_argument, uint64_t** failed_cpu_list))
    PROTOCOL_METHOD(mp_whoami, (struct _mp_services_protocol_t* _this, uint64_t* processor_number))
    typedef struct _mp_services_protocol_t
    {
        mp_get_number_of_processors get_number_of_processors;
        mp_get_number_of_processors fill1;
        mp_startup_all_aps startup_all_aps;
        mp_get_number_of_processors fill2;
        mp_get_number_of_processors fill3;
        mp_get_number_of_processors fill4;
        mp_whoami whoami;
    } mp_services_protocol_t;
}

