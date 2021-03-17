#pragma once

#include "efi/core.hh"
#include "efi/protocol/macro.hh"

namespace kyu::efi::protocol
{
    typedef struct _input_key_t
    {
        uint16_t scan_code;
        uint16_t unicode_char;
    } input_key_t;

    struct _simple_text_input_protocol_t;

    PROTOCOL_METHOD(stip_reset_t, (struct _simple_text_input_protocol_t* _this, bool_t extended_verification))
    PROTOCOL_METHOD(stip_read_key_stroke_t, (struct _simple_text_input_protocol_t* _this, input_key_t* key))

    typedef struct _simple_text_input_protocol_t
    {
        stip_reset_t reset;
        stip_read_key_stroke_t read_key_stroke;
        event_t wait_for_key;
    } simple_text_input_protocol_t;
}