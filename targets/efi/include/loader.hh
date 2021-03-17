#pragma once

#include "graphics.hh"
#include "efi/system-table.hh"
#include "efi/protocol/graphics-output.hh"

namespace kyu::loader
{
    int RunHypervisor();
    void LoadHypervisorFromFS();
    graphics::image_t* LoadFFImageFromFS(wchar_t* image_path);
}