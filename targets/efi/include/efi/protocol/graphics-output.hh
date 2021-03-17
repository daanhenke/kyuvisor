#pragma once

#include "efi/core.hh"
#include "efi/protocol/macro.hh"

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID   {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}}

namespace kyu::efi::protocol
{
    typedef struct _pixel_bitmask_t
    {
        uint32_t red_mask;
        uint32_t green_mask;
        uint32_t blue_mask;
        uint32_t reserved_mask;
    } pixel_bitmask_t;

    typedef enum _graphics_pixel_format_t
    {
        pixel_rgbr8bpc,
        pixel_bgrr8bpc,
        pixel_bit_mask,
        pixel_blt_only,
        pixel_format_mask
    } graphics_pixel_format_t;

    typedef struct _graphics_output_mode_information_t
    {
        uint32_t version;
        uint32_t horizontal_resolution;
        uint32_t vertical_resolution;
        graphics_pixel_format_t pixel_format;
        pixel_bitmask_t pixel_information;
        uint32_t pixels_per_scanline;

    } graphics_output_mode_information_t;

    typedef struct _graphics_output_protocol_mode_t
    {
        uint32_t max_mode;
        uint32_t mode;
        graphics_output_mode_information_t* info;
        uint64_t size_of_info;
        physical_address_t framebuffer_base;
        uint64_t framebuffer_size;
    } graphics_output_protocol_mode_t;

    typedef struct _graphics_output_blt_pixel_t
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t reserved;
    } graphics_output_blt_pixel_t;

    typedef enum _graphics_output_blt_operation_t
    {
        blt_video_fill,
        blt_video_to_blt_buffer,
        blt_buffer_to_video,
        blt_video_to_video
    } graphics_output_blt_operation_t;

    struct _graphics_output_protocol_t;

    PROTOCOL_METHOD(gop_query_mode_t, (struct _graphics_output_protocol_t* _this, uint32_t mode_number, uint64_t* size_of_info, graphics_output_mode_information_t **info))
    PROTOCOL_METHOD(gop_set_mode_t, (struct _graphics_output_protocol_t* _this, uint32_t mode_number))
    PROTOCOL_METHOD(gop_blt_t, (struct _graphics_output_protocol_t* _this, graphics_output_blt_pixel_t* blt_buffer, graphics_output_blt_operation_t blt_operation, uint64_t source_x, uint64_t source_y, uint64_t dest_x, uint64_t dest_y, uint64_t width, uint64_t height, uint64_t delta))

    typedef struct _graphics_output_protocol_t
    {
        gop_query_mode_t query_mode;
        gop_set_mode_t set_mode;
        gop_blt_t blt;
        graphics_output_protocol_mode_t* mode;
    } graphics_output_protocol_t;
}