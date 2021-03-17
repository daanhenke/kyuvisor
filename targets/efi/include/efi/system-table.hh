#pragma once

#include "efi/core.hh"
#include "efi/boot-services.hh"
#include "efi/runtime-services.hh"
#include "efi/protocol/simple-text-input.hh"
#include "efi/protocol/simple-text-output.hh"

namespace kyu::efi
{
    typedef struct _system_table_t
    {
        table_header_t header;
        echar16_t* firmware_vendor;
        uint32_t firmware_revision;
        handle_t handle_console_in;
        protocol::simple_text_input_protocol_t* console_in;
        handle_t handle_console_out;
        protocol::simple_text_output_protocol_t* console_out;
        handle_t handle_standard_error;
        protocol::simple_text_output_protocol_t* standard_error;
        runtime_services_t* runtime_services;
        boot_services_t* boot_services;
        uint64_t number_of_table_entries;
        configuration_table_t* configuration_table;
    } system_table_t;

    extern system_table_t* system_table;
}