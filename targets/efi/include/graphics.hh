#pragma once

#include "efi/system-table.hh"
#include "efi/protocol/graphics-output.hh"

namespace kyu::graphics
{
    typedef struct _image_t
    {
        uint64_t width;
        uint64_t height;
        efi::protocol::graphics_output_blt_pixel_t* data;
    } image_t;

    void InitializeGraphics();
    void LoadFont();
    void DrawBackground();
    void DrawConsole();
    void Panic(int code);
}