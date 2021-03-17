#pragma once

#include "efi/protocol/macro.hh"

namespace kyu::efi::protocol
{
    struct _simple_text_output_protocol_t;

    PROTOCOL_METHOD(stop_reset_t, (struct _simple_text_output_protocol_t* _this, bool_t extended_verification))
    PROTOCOL_METHOD(stop_output_string_t, (struct _simple_text_output_protocol_t* _this, echar16_t* string))
    PROTOCOL_METHOD(stop_clear_screen_t, (struct _simple_text_output_protocol_t* _this))

    typedef struct _simple_text_output_protocol_t
    {
        stop_reset_t reset;
        stop_output_string_t output_string;
        stop_reset_t test_string;
        stop_reset_t query_mode;
        stop_reset_t set_mode;
        stop_reset_t set_attribute;
        stop_clear_screen_t clear_screen;
    } simple_text_output_protocol_t;
}